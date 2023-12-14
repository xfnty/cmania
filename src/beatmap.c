#include "beatmap.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <kvec.h>
#include <khash.h>
#include <ksort.h>

#include "file.h"
#include "error.h"
#include "kvec.h"
#include "logging.h"

// Calculated using kh_str_hash_func("Whatever")
#define SECTION_DIFFICULTY     -472001573
#define SECTION_TIMING_POINTS  -442323475
#define SECTION_HITOBJECTS     -1760687583
#define KEY_SV                 -215404126
#define KEY_CS                 882574609


static const char* skip_over_whitespace(const char* s);
static const char* skip_to(const char* s, char c);
static bool sort_events_callback(beatmap_event_t a, beatmap_event_t b);

KSORT_INIT(beatmap_event_t, beatmap_event_t, sort_events_callback);

err_t beatmap_create_from_str(beatmap_t* beatmap, const char* const str) {
    ASSERT(beatmap != NULL);
    ASSERT(str != NULL);

    *beatmap = (beatmap_t){0};

    beatmap_event_t last_timing_point = {0};
    const char* cursor = str;
    int current_section = 0;
    while (*cursor != '\0') {
        cursor = skip_over_whitespace(cursor);

        if (*cursor == '[') {
            cursor++;
            const char* closing_bracket = strchr(cursor, ']');
            char section_name[64] = {0};
            strncpy(section_name, cursor, MIN(closing_bracket - cursor, LENGTH(section_name) - 1));
            current_section = kh_str_hash_func(section_name);
        }
        else {
            switch (current_section) {
                case SECTION_DIFFICULTY: {
                    char key[64] = {0}, value[64] = {0};
                    
                    const char* colon = skip_to(cursor, ':');
                    strncpy(key, cursor, MIN(colon - cursor, LENGTH(key) - 1));
                    khint_t key_hash = kh_str_hash_func(key);
                    cursor = skip_over_whitespace(colon + 1);
                    
                    const char* chr_after_value = cursor;
                    while ((*chr_after_value >= '0' && *chr_after_value <= '9') || *chr_after_value == '.') {
                        chr_after_value++;
                    }
                    strncpy(value, cursor, MIN(chr_after_value - cursor, LENGTH(value) - 1));

                    if (key_hash == KEY_CS) {
                        float CS = atof(value);
                        if (floorf(CS) != CS)
                            return ERROR_INVALID_VALUE;

                        ASSERT_LOG_RETURN_VALUE(CS > 0, ERROR_UNDEFINED, "Failed to parse CS value (\"%s\")", value);
                        ASSERT_LOG_RETURN_VALUE(kv_size(beatmap->columns) == 0, ERROR_UNDEFINED, "Redifinition of CS parameter");

                        kv_init(beatmap->columns);
                        kv_resize(beatmap_column_t, beatmap->columns, (int)CS);
                        for (int i = 0; i < kv_size(beatmap->columns); i++) {
                            kv_init(kv_A(beatmap->columns, i));
                        }
                    }
                    else if (key_hash == KEY_SV) {
                        beatmap->base_velocity = atof(value);
                    }
                    break;
                }

                case SECTION_TIMING_POINTS: {
                    float time, beat_length, meter, sample_set, sample_index, volume, uninherited, effects;
                    int args = sscanf(
                        cursor, "%f,%f,%f,%f,%f,%f,%f,%f",
                        &time, &beat_length, &meter, &sample_set, &sample_index, &volume, &uninherited, &effects
                    );
                    if (args != 8)
                        break;

                    time /= 1000;

                    ASSERT_LOG_RETURN_VALUE(
                        !(last_timing_point.type == BEATMAP_EVENT_INVALID && !uninherited),
                        ERROR_UNDEFINED,
                        "First timing point can not be uninherited"
                    );

                    beatmap_event_t timing_point = {
                        .type = BEATMAP_EVENT_TIMING_POINT,
                        .time = time,
                        .BPM = (uninherited) ? (roundf(60000.0f / beat_length)) : (last_timing_point.BPM),
                        .SV = (uninherited) ? (beatmap->base_velocity) : (beatmap->base_velocity * (100.0f / -beat_length)),
                        .position = last_timing_point.position + 100 * last_timing_point.SV * (last_timing_point.BPM / 60) * (time - last_timing_point.time),
                    };

                    for (size_t i = 0; i < kv_size(beatmap->columns); i++) {
                        kv_push(beatmap_event_t, kv_A(beatmap->columns, i), timing_point);
                    }
                    
                    last_timing_point = timing_point;
                    break;
                }

                case SECTION_HITOBJECTS: {
                    float x, y, time, type, hitsound, endtime;
                    int args = sscanf(cursor, "%f,%f,%f,%f,%f,%f", &x, &y, &time, &type, &hitsound, &endtime);
                    if (args != 6)
                        continue;

                    time /= 1000;
                    endtime /= 1000;

                    ASSERT_LOG_RETURN_VALUE(kv_size(beatmap->columns) > 0, ERROR_UNDEFINED, "Can not add hitobjects with specifying CS first");
                    int column = (int)floorf(x * kv_size(beatmap->columns) / 512.0f);
                    ASSERT_LOG_RETURN_VALUE(column >= 0 && column < kv_size(beatmap->columns), ERROR_UNDEFINED, "Hitobject's column (%d) is out of range (0 to %lu)", column, kv_size(beatmap->columns));

                    if (type == 128) {
                        beatmap_event_t hold_start = {
                            .type = BEATMAP_EVENT_HOLD_START,
                            .time = time,
                            .BPM = 0,
                            .SV = 0,
                        };
                        beatmap_event_t hold_end = {
                            .type = BEATMAP_EVENT_HOLD_END,
                            .time = endtime,
                            .BPM = 0,
                            .SV = 0,
                        };

                        kv_push(beatmap_event_t, kv_A(beatmap->columns, column), hold_start);
                        kv_push(beatmap_event_t, kv_A(beatmap->columns, column), hold_end);
                    }
                    else {
                        beatmap_event_t click = {
                            .type = BEATMAP_EVENT_CLICK,
                            .time = time,
                            .BPM = 0,
                            .SV = 0,
                        };
                        
                        kv_push(beatmap_event_t, kv_A(beatmap->columns, column), click);
                    }
                }
            }
        }

        cursor = skip_to(cursor, '\n');
        if (*cursor != '\0')
            cursor++;
    }

    ASSERT_LOG_RETURN_VALUE(kv_size(beatmap->columns) != 0, ERROR_UNDEFINED, "CS is unset");
    ASSERT_LOG_RETURN_VALUE(beatmap->base_velocity != 0, ERROR_UNDEFINED, "SV is unset");

    for (size_t i = 0; i < kv_size(beatmap->columns); i++) {
        beatmap_column_t* col = &kv_A(beatmap->columns, i);
        ks_introsort(beatmap_event_t, kv_size(*col), col->a);
    }
    
    for (size_t i = 0; i < kv_size(beatmap->columns); i++) {
        beatmap_column_t col = kv_A(beatmap->columns, i);

        beatmap_event_t last_tm = { .type = BEATMAP_EVENT_INVALID };
        for (size_t j = 0; j < kv_size(col); j++) {
            beatmap_event_t* ev = &kv_A(col, j);

            if (ev->type == BEATMAP_EVENT_TIMING_POINT) {
                last_tm = *ev;
                continue;
            }

            ASSERT(ev->type > BEATMAP_EVENT_INVALID && ev->type < _BEATMAP_EVENT_COUNT);
            ASSERT_LOG_RETURN_VALUE(last_tm.type != BEATMAP_EVENT_INVALID, ERROR_INVALID_VALUE, "Click event can not appear before Timing Point");

            ev->position = last_tm.position + 100 * last_tm.SV * (last_tm.BPM / 60) * (ev->time - last_tm.time);
        }
    }

    return ERROR_SUCCESS;
}

err_t beatmap_create_from_file(beatmap_t* beatmap, const char* path) {
    ASSERT(path != NULL);
    ASSERT(beatmap != NULL);

    file_t f = {0};
    CHECK_ERROR_PROPAGATE(file_read(&f, path));

    err_t err = beatmap_create_from_str(beatmap, f.data);
    file_free(&f);

    return err;
}

void beatmap_destroy(beatmap_t* beatmap) {
    ASSERT(beatmap != NULL);

    for (size_t i = 0; i < kv_size(beatmap->columns); i++)
        kv_destroy(kv_A(beatmap->columns, i));
    kv_destroy(beatmap->columns);
    kv_init(beatmap->columns);

    beatmap->base_velocity = 0;
}

void beatmap_print(const beatmap_t* beatmap) {
    ASSERT(beatmap != NULL);

    LOG("Beatmap summary:");
    PRINTLN("  columns=%lu base_SV=%.1f", kv_size(beatmap->columns), beatmap->base_velocity);

    size_t event_count = 0;
    for (size_t i = 0; i < kv_size(beatmap->columns); i++)
        event_count += kv_size(kv_A(beatmap->columns, i));
    PRINTLN("  events[%lu]:", event_count);

    static const char* event_type_names[] = {
        "XX", "TM", "CK", "HS", "HE"
    };
    for (size_t ci = 0; ci < kv_size(beatmap->columns); ci++) {
        PRINTLN("    column %lu:", ci);
        beatmap_column_t* col = &kv_A(beatmap->columns, ci);
        for (size_t ei = 0; ei < kv_size(*col); ei++) {
            beatmap_event_t* ev = &kv_A(*col, ei);
            ASSERT(ev->type >= 0 && ev->type < _BEATMAP_EVENT_COUNT);
            PRINT("      %s at %.2f (%.1f)", event_type_names[ev->type], ev->time, ev->position);
            if (ev->type == BEATMAP_EVENT_TIMING_POINT)
                PRINT(" BPM=%.1f SV=%.1f", ev->BPM, ev->SV);
            PRINTLN();
        }
    }
}

const char* skip_over_whitespace(const char* s) {
    ASSERT(s != NULL);
    while (*s == ' ' || *s == '\t' || *s == '\n' || *s == '\r') s++;
    return s;
}

const char* skip_to(const char* s, char c) {
    ASSERT(s != NULL);
    while (*s != '\0' && *s != c) {
        s++;
    }
    return s;
}

bool sort_events_callback(beatmap_event_t a, beatmap_event_t b) {
    return
        a.time < b.time
        || (
            a.time == b.time
            && a.type == BEATMAP_EVENT_TIMING_POINT
            && b.type != BEATMAP_EVENT_TIMING_POINT
        );
}

#include "osu_beatmap.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <kvec.h>
#include <khash.h>

#include "file.h"
#include "util.h"
#include "error.h"
#include "logging.h"

// Calculated using kh_str_hash_func("Whatever")
#define SECTION_DIFFICULTY     -472001573
#define SECTION_TIMING_POINTS  -442323475
#define SECTION_HITOBJECTS     -1760687583
#define KEY_SV                 -215404126
#define KEY_CS                 882574609


static const char* skip_over_whitespace(const char* s);
static const char* skip_to(const char* s, char c);

err_t osu_beatmap_create_from_str(osu_beatmap_t* beatmap, const char* const str) {
    ASSERT(beatmap != NULL);
    ASSERT(str != NULL);

    *beatmap = (osu_beatmap_t){0};

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
                        beatmap->CS = atof(value);
                    }
                    else if (key_hash == KEY_SV) {
                        beatmap->SV = atof(value);
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

                    osu_timing_point_t prev_tm = (kv_size(beatmap->timing_points) > 0)
                        ? (kv_A(beatmap->timing_points, kv_size(beatmap->timing_points) - 1))
                        : ((osu_timing_point_t){0});

                    osu_timing_point_t tm = {
                        .start = time / 1000.0f,
                        .BPM = (uninherited) ? (roundf(60000.0f / beat_length)) : (prev_tm.BPM),
                        .SV = (uninherited) ? (beatmap->SV) : (beatmap->SV * (100.0f / -beat_length)),
                    };

                    kv_push(osu_timing_point_t, beatmap->timing_points, tm);
                    break;
                }

                case SECTION_HITOBJECTS: {
                    int x, y, time, type, hitsound, endtime;
                    int args = sscanf(cursor, "%d,%d,%d,%d,%d,%d", &x, &y, &time, &type, &hitsound, &endtime);
                    if (args != 6)
                        continue;

                    osu_hitobject_t ho = {
                        .start = time / 1000.0f,
                        .end = (type == 128) ? (endtime / 1000.0f) : (-1),
                        .column = (int)floorf(x * beatmap->CS / 512)
                    };

                    kv_push(osu_hitobject_t, beatmap->hitobjects, ho);
                }
            }
        }

        cursor = skip_to(cursor, '\n');
        if (*cursor != '\0')
            cursor++;
    }

    ASSERT_LOG_RETURN_VALUE(beatmap->CS != 0, ERROR_UNDEFINED, "CS is unset");
    ASSERT_LOG_RETURN_VALUE(beatmap->SV != 0, ERROR_UNDEFINED, "SV is unset");
    ASSERT_LOG_RETURN_VALUE(kv_size(beatmap->timing_points) > 0, ERROR_UNDEFINED, "no timing points");
    ASSERT_LOG_RETURN_VALUE(kv_size(beatmap->hitobjects) > 0, ERROR_UNDEFINED, "no hitobjects");

    return ERROR_SUCCESS;
}

err_t osu_beatmap_create_from_file(osu_beatmap_t* beatmap, const char* path) {
    ASSERT(beatmap != NULL);
    ASSERT(path != NULL);
    file_t file = {0};
    CHECK_ERROR_PROPAGATE(file_read(&file, path));
    err_t err = osu_beatmap_create_from_str(beatmap, file.data);
    file_free(&file);
    return err;
}

void osu_beatmap_destroy(osu_beatmap_t* beatmap) {
    ASSERT(beatmap != NULL);

    kv_destroy(beatmap->hitobjects);
    kv_destroy(beatmap->timing_points);
    kv_init(beatmap->hitobjects);
    kv_init(beatmap->timing_points);
}

void osu_beatmap_print(const osu_beatmap_t* beatmap) {
    ASSERT(beatmap != NULL);

    LOG("Osu beatmap summary:");
    PRINTLN("  CS=%.1f SV=%.1f", beatmap->CS, beatmap->SV);
    PRINTLN("  hitobjects[%lu]", kv_size(beatmap->hitobjects));
    for (int i = 0; i < kv_size(beatmap->hitobjects); i++) {
        osu_hitobject_t* ho = &kv_A(beatmap->hitobjects, i);
        PRINTLN("    %d: start=%.2f end=%.2f col=%d", i, ho->start, ho->end, ho->column);
    }
    PRINTLN("  timing points[%lu]", kv_size(beatmap->timing_points));
    for (int i = 0; i < kv_size(beatmap->timing_points); i++) {
        osu_timing_point_t* tm = &kv_A(beatmap->timing_points, i);
        PRINTLN("    %d: start=%.2f SV=%.2f BPM=%.2f", i, tm->start, tm->SV, tm->BPM);
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

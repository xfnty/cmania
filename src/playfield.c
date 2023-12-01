#define SCOPE_NAME "playfield loader"
#include "playfield.h"

#include <assert.h>

#include <kvec.h>

#include "util.h"
#include "beatmap.h"
#include "logging.h"


error_t playfield_create_from(difficulty_t* difficulty, playfield_t* playfield) {
    assert(difficulty != NULL);
    assert(playfield != NULL);

    kv_init(playfield->speed_mods);

    for (int i = 0; i < kv_size(difficulty->timing_points); i++) {
        timing_point_t* tm = &kv_A(difficulty->timing_points, i);
        
        float position = 0;
        if (i > 0) {
            timing_point_t* ptm = &kv_A(difficulty->timing_points, i - 1);
            playfield_speed_modifier_t* psm = &kv_A(playfield->speed_mods, kv_size(playfield->speed_mods) - 1);
            position = psm->position + (tm->time - ptm->time) * 100 * ptm->SV / (60.0f / ptm->BPM);
        }

        playfield_speed_modifier_t sm = {
            .speed = 100 * tm->SV / (60.0f / tm->BPM),
            .position = position,
        };
        kv_push(playfield_speed_modifier_t, playfield->speed_mods, sm);
    }

    kv_init(playfield->columns);
    kv_resize(playfield_column_t, playfield->columns, (int)difficulty->CS);
    kv_size(playfield->columns) = (int)difficulty->CS;
    for (int i = 0; i < kv_size(playfield->columns); i++)
        kv_init(kv_A(playfield->columns, i).events);
    for (int i = 0; i < kv_size(difficulty->hitobjects); i++) {
        hitobject_t* ho = &kv_A(difficulty->hitobjects, i);

        playfield_event_t pe = {
            .position = ho->start_time,
        };

        if (ho->end_time) {
            pe.type = PLAYFIELD_EVENT_HOLD_BEGIN;
            kv_push(playfield_event_t, kv_A(playfield->columns, ho->column).events, pe);
            pe.type = PLAYFIELD_EVENT_HOLD_END;
            pe.position = ho->end_time;
            kv_push(playfield_event_t, kv_A(playfield->columns, ho->column).events, pe);
        }
        else {
            pe.type = PLAYFIELD_EVENT_NOTE;
            kv_push(playfield_event_t, kv_A(playfield->columns, ho->column).events, pe);
        }
    }

    LOG_SUCCESS("Created playfield from \"%s\"", difficulty->name);
    return ERROR_SUCCESS;
}

void playfield_destroy(playfield_t* playfield) {
}

void playfield_debug_print(playfield_t* playfield) {
    static const char* event_names[] = {
        "INV", "NOTE", "HOLD_BEG", "HOLD_END"
    };

    LOG("Playfield info:");
    int c = 0;
    for (int i = 0; i < kv_size(playfield->columns); i++)
        c += kv_size(kv_A(playfield->columns, i).events);
    PRINTLN("events[%d]:", c);
    for (int i = 0; i < kv_size(playfield->columns); i++) {
        playfield_column_t* pc = &kv_A(playfield->columns, i);

        for (int j = 0; j < kv_size(pc->events); j++) {
            playfield_event_t* pe = &kv_A(pc->events, j);
            PRINTLN("\tEV[%d]: %s at %8f", j, event_names[pe->type], pe->position);
        }
    }

    PRINTLN("spd mods[%lu]:", kv_size(playfield->speed_mods));
    for (int i = 0; i < kv_size(playfield->speed_mods); i++) {
        playfield_speed_modifier_t* sm = &kv_A(playfield->speed_mods, i);
        PRINTLN("\tSM[%d]: %8.f at %8.f", i, sm->speed, sm->position);
    }
}

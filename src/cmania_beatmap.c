#include <cmania_beatmap.h>

#include <kvec.h>

#include "error.h"
#include "osu_beatmap.h"


err_t cmania_beatmap_create_from_osu_beatmap(cmania_beatmap_t* out_beatmap, const osu_beatmap_t* in_beatmap) {
    ASSERT(out_beatmap != NULL);
    ASSERT(in_beatmap != NULL);

    *out_beatmap = (cmania_beatmap_t){0};

    out_beatmap->column_count = in_beatmap->CS;

    for (int i = 0; i < kv_size(in_beatmap->timing_points); i++) {
        osu_timing_point_t* tm = &kv_A(in_beatmap->timing_points, i);
        osu_timing_point_t ptm = 
            (i > 0)
            ? (kv_A(in_beatmap->timing_points, i - 1))
            : ((osu_timing_point_t){0});
        cmania_speed_modifier_t psm = 
            (kv_size(out_beatmap->speed_modifiers) > 0)
            ? (kv_A(out_beatmap->speed_modifiers, kv_size(out_beatmap->speed_modifiers) - 1))
            : ((cmania_speed_modifier_t){0});

        cmania_speed_modifier_t sm = {
            .start = psm.start + psm.speed * (tm->start - ptm.start),
            .speed = tm->SV * 100 * (tm->BPM / 60),
        };
        kv_push(cmania_speed_modifier_t, out_beatmap->speed_modifiers, sm);
    }


    int ptm_i = -1;
    for (int i = 0; i < kv_size(in_beatmap->hitobjects); i++) {
        osu_hitobject_t* ho = &kv_A(in_beatmap->hitobjects, i);

        while (ptm_i + 1 < kv_size(in_beatmap->timing_points) && kv_A(in_beatmap->timing_points, ptm_i + 1).start < ho->start)
            ptm_i++;
        ASSERT(ptm_i >= 0 && ptm_i < kv_size(in_beatmap->timing_points));
        osu_timing_point_t* tm = &kv_A(in_beatmap->timing_points, ptm_i+1);

        cmania_speed_modifier_t sm = kv_A(out_beatmap->speed_modifiers, ptm_i+1);  // speed modifier associated with `tm`

        cmania_hitobject_t cmho = {
            .column = ho->column,
            .start = sm.start + sm.speed * (ho->start - tm->start),
            .end = -1
        };

        if (ho->end != -1) {
            // calculate distance between `start` and `end`
            // even if there are multiple timing points between them
        }

        kv_push(cmania_hitobject_t, out_beatmap->hitobjects, cmho);
    }

    return ERROR_SUCCESS;
}

void cmania_beatmap_destroy(cmania_beatmap_t* beatmap) {
    ASSERT(beatmap != NULL);

    kv_destroy(beatmap->hitobjects);
    kv_destroy(beatmap->speed_modifiers);
    kv_init(beatmap->hitobjects);
    kv_init(beatmap->speed_modifiers);
}

void cmania_beatmap_print(const cmania_beatmap_t* beatmap) {
    LOG("CMania beatmap summary:");
    PRINTLN("  cols=%d", beatmap->column_count);
    PRINTLN("  hitobjects[%lu]", kv_size(beatmap->hitobjects));
    for (int i = 0; i < kv_size(beatmap->hitobjects); i++) {
        cmania_hitobject_t* ho = &kv_A(beatmap->hitobjects, i);
        PRINTLN("    %d: start=%.2f end=%.2f col=%d", i, ho->start, ho->end, ho->column);
    }
    PRINTLN("  speed modifiers[%lu]", kv_size(beatmap->speed_modifiers));
    for (int i = 0; i < kv_size(beatmap->speed_modifiers); i++) {
        cmania_speed_modifier_t* sm = &kv_A(beatmap->speed_modifiers, i);
        PRINTLN("    %d: start=%.2f speed=%.2f", i, sm->start, sm->speed);
    }
}

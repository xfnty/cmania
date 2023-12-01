#ifndef CMANIA_BEATMAP_H_
#define CMANIA_BEATMAP_H_

#include <kvec.h>

#include "error.h"
#include "types.h"
#include "osu_beatmap.h"


typedef struct {
    float start;
    float end;
    int   column;
} cmania_hitobject_t;

typedef struct {
    seconds_t start;
    float     speed;
} cmania_speed_modifier_t;

typedef struct {
    char name[128];
    char audio_filename[128];

    int column_count;

    kvec_t(cmania_hitobject_t)      hitobjects;
    kvec_t(cmania_speed_modifier_t) timing_points;
} cmania_beatmap_t;

typedef struct {
    char title[128];
    char artist[128];

    kvec_t(cmania_beatmap_t) beatmaps;
} cmania_beatmapset_t;


error_t cmania_beatmap_create_from_osu_beatmap(cmania_beatmap_t* out_beatmap, const osu_beatmap_t* in_beatmap);
void cmania_beatmap_destroy(cmania_beatmap_t* beatmap);
void cmania_beatmap_print(const cmania_beatmap_t* beatmap);


#endif

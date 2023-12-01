#ifndef OSU_BEATMAP_H_
#define OSU_BEATMAP_H_

#include <kvec.h>

#include "types.h"
#include "error.h"


typedef struct {
    seconds_t start;
    seconds_t end;
    int       column;
} osu_hitobject_t;

typedef struct {
    seconds_t start;
    float SV;
    float BPM;
    bool  is_kiai;
    float sound_volume;
} osu_timing_point_t;

typedef struct {
    seconds_t start;
    seconds_t end;
} osu_break_t;

typedef struct {
    int  id;
    char name[128];
    char audio_filename[128];

    float HP;
    float CS;
    float OD;
    float AR;
    float SV;

    kvec_t(osu_break_t) breaks;
    kvec_t(osu_hitobject_t) hitobjects;
    kvec_t(osu_timing_point_t) timing_points;
} osu_beatmap_t;

typedef struct {
    int  id;
    char title[128];
    char artist[128];
    char creator[128];
    char source[128];

    kvec_t(osu_beatmap_t) beatmaps;
} osu_beatmapset_t;


error_t osu_beatmap_create_from_file(osu_beatmap_t* beatmap, const char* filepath);
void    osu_beatmap_destroy(osu_beatmap_t* beatmap);
void    osu_beatmap_print(const osu_beatmap_t* beatmap);


#endif

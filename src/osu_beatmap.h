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
} osu_timing_point_t;

typedef struct {
    float CS;
    float SV;

    kvec_t(osu_hitobject_t) hitobjects;
    kvec_t(osu_timing_point_t) timing_points;
} osu_beatmap_t;


#ifdef __cplusplus
extern "C" {
#endif

err_t osu_beatmap_create_from_str(osu_beatmap_t* beatmap, const char* const str);
void  osu_beatmap_destroy(osu_beatmap_t* beatmap);
void  osu_beatmap_print(const osu_beatmap_t* beatmap);

#ifdef __cplusplus
};
#endif


#endif

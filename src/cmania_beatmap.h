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
    float start;
    float speed;
} cmania_speed_modifier_t;

typedef struct {
    int column_count;

    kvec_t(cmania_hitobject_t)      hitobjects;
    kvec_t(cmania_speed_modifier_t) speed_modifiers;
} cmania_beatmap_t;


#ifdef __cplusplus
extern "C" {
#endif

err_t cmania_beatmap_create_from_osu_beatmap(cmania_beatmap_t* out_beatmap, const osu_beatmap_t* in_beatmap);
void cmania_beatmap_destroy(cmania_beatmap_t* beatmap);
void cmania_beatmap_print(const cmania_beatmap_t* beatmap);

#ifdef __cplusplus
}
#endif


#endif

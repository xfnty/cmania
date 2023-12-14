#ifndef BEATMAP_H_
#define BEATMAP_H_

#include <kvec.h>

#include "types.h"
#include "error.h"


typedef enum {
    BEATMAP_EVENT_INVALID,
    BEATMAP_EVENT_TIMING_POINT,
    BEATMAP_EVENT_CLICK,
    BEATMAP_EVENT_HOLD_START,
    BEATMAP_EVENT_HOLD_END,
    _BEATMAP_EVENT_COUNT,
} beatmap_event_type_t;

typedef struct {
    beatmap_event_type_t type;
    f32 position;
    f32 time;
    // Used for TIMING_POINT, 0 otherwise
    f32 BPM;
    f32 SV;
} beatmap_event_t;

typedef kvec_t(beatmap_event_t) beatmap_column_t;

typedef struct {
    f32 base_velocity;
    kvec_t(beatmap_column_t) columns;
} beatmap_t;


#ifdef __cplusplus
extern "C" {
#endif

err_t beatmap_create_from_str(beatmap_t* beatmap, const char* const str);
err_t beatmap_create_from_file(beatmap_t* beatmap, const char* path);
void  beatmap_destroy(beatmap_t* beatmap);
void  beatmap_print(const beatmap_t* beatmap);

#ifdef __cplusplus
};
#endif


#endif

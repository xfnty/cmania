#ifndef PLAYFIELD_H
#define PLAYFIELD_H

#include <stdbool.h>

#include <kvec.h>
#include <raylib.h>

#include "util.h"
#include "beatmap.h"


/* types */
typedef enum {
    PLAYFIELD_EVENT_INVALID,
    PLAYFIELD_EVENT_NOTE,
    PLAYFIELD_EVENT_HOLD_BEGIN,
    PLAYFIELD_EVENT_HOLD_END,
} playfield_event_type_t;

typedef struct {
    float                   position;  // Y coordinate
    playfield_event_type_t  type;
} playfield_event_t;

typedef struct {
    float position;
    float speed;  // opx
} playfield_speed_modifier_t;

typedef struct {
    kvec_t(playfield_event_t) events;
} playfield_column_t;

typedef struct {
    kvec_t(playfield_column_t)          columns;
    kvec_t(playfield_speed_modifier_t)  speed_mods;
} playfield_t;


/* function declarations */
error_t playfield_create_from(difficulty_t* difficulty, playfield_t* playfield);
void    playfield_destroy(playfield_t* playfield);
void    playfield_debug_print(playfield_t* playfield);


#endif

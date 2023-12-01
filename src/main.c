#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#include <kvec.h>
#include <raylib.h>

#include "util.h"
#include "beatmap.h"
#include "playfield.h"


static beatmap_t beatmap;
static playfield_t playfield;

int main(int argc, const char *argv[]) {
    logging_init();

    if (argc <= 1) {
        printf("Usage: %s <.osz/folder>\n", GetFileName(argv[0]));
        exit(0);
    }

    CHECK_ERROR(beatmap_load(&beatmap, argv[1]));
    beatmap_debug_print(&beatmap);

    CHECK_ERROR(playfield_create_from(&kv_A(beatmap.difficulties, 0), &playfield));
    playfield_debug_print(&playfield);

    return 0;
}

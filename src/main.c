#include "error.h"
#include "beatmap.h"
#include "logging.h"


int main(int argc, const char *argv[]) {
    if (argc <= 1) {
        PRINTLN(ANSI_COLOR_WHITE "usage: cmania <.osu file>");
        return 0;
    }

    beatmap_t beatmap = {0};
    CHECK_ERROR_LOG_RETURN_VALUE(beatmap_create_from_file(&beatmap, argv[1]), -1, "Could not load \"%s\"", argv[1]);

    beatmap_print(&beatmap);

    beatmap_destroy(&beatmap);
    return 0;
}

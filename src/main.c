#include "file.h"
#include "util.h"
#include "error.h"
#include "types.h"
#include "logging.h"
#include "osu_beatmap.h"


int main(int argc, const char *argv[]) {
    if (argc <= 1) {
        PRINTLN(ANSI_COLOR_WHITE_BOLD "usage: cmania <.osu file>");
        return 0;
    }

    osu_beatmap_t bm = {0};
    CHECK_ERROR_LOG_RETURN_VALUE(osu_beatmap_create_from_file(&bm, argv[1]), -1, "could not parse beatmap file");

    osu_beatmap_print(&bm);
    osu_beatmap_destroy(&bm);
    return 0;
}

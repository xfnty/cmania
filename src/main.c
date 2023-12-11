#include "file.h"
#include "util.h"
#include "error.h"
#include "types.h"
#include "logging.h"
#include "osu_beatmap.h"
#include "cmania_beatmap.h"


int main(int argc, const char *argv[]) {
    if (argc <= 1) {
        PRINTLN(ANSI_COLOR_WHITE "usage: cmania <.osu file>");
        return 0;
    }

    osu_beatmap_t bm = {0};
    CHECK_ERROR_LOG_RETURN_VALUE(osu_beatmap_create_from_file(&bm, argv[1]), -1, "could not parse beatmap file");
    osu_beatmap_print(&bm);

    cmania_beatmap_t cbm = {0};
    CHECK_ERROR_LOG_RETURN_VALUE(cmania_beatmap_create_from_osu_beatmap(&cbm, &bm), -2, "failed to convert Osu beatmap to CMania beatmap");
    cmania_beatmap_print(&cbm);

    osu_beatmap_destroy(&bm);
    cmania_beatmap_destroy(&cbm);
    return 0;
}

#include "osu_beatmap.h"

#include "error.h"
#include "logging.h"


error_t osu_beatmap_create_from_file(osu_beatmap_t* beatmap, const char* filepath) {
    return ERROR_SUCCESS;
}

void osu_beatmap_destroy(osu_beatmap_t* beatmap) {
}

void osu_beatmap_print(const osu_beatmap_t* beatmap) {
    ASSERT(beatmap != NULL);

    LOG("Osu beatmap summary:");
    PRINTLN("  id: %d", beatmap->id);
    PRINTLN("  name: \"%s\"", beatmap->name);
    PRINTLN("  audio: \"%s\"", beatmap->audio_filename);
    PRINTLN(
        "  HP=%.1f CS=%.1f OD=%.1f AR=%.1f SV=%.1f",
        beatmap->HP, beatmap->CS, beatmap->OD, beatmap->AR, beatmap->SV
    );
}

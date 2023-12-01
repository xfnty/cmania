#include "util.h"
#include "types.h"
#include "osu_beatmap.h"


int main(int argc, const char *argv[]) {
    osu_beatmap_t bm = {
        .AR = 1,
        .CS = 2,
        .OD = 3,
        .SV = 4,
        .HP = 5,
        .id = 10,
        .name = "Sample beatmap",
        .audio_filename = "audio filepath",
    };
    osu_beatmap_print(&bm);
    return 0;
}

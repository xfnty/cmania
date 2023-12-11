#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>

#include <filesystem>

#include <kvec.h>

#include "file.h"
#include "error.h"
#include "osu_beatmap.h"
#include "cmania_beatmap.h"


TEST_CASE("Osu to CMania beatmap conversion") {
    const char* text =
    "osu file format v14\n"
    "[General]\n"
    "Mode: 3\n"
    "[Difficulty]\n"
    "CircleSize:  4\n"
    "SliderMultiplier:2.5\n"
    "[TimingPoints]\n"
    "0,333.33,4,0,0,100,1,0 # BPM 180\n"
    "2000,-50,4,0,0,100,0,0 # BPM 100\n"
    "[HitObjects]\n"
    "256,192,1500,0,0,0:0:0:0:      # COL=2 time=1.5s\n"
    "384,192,2500,128,0,3500:0:0:0: # COL=3 start=2.5s end=3.5s\n"
    ;
    osu_beatmap_t bm = {0};

    err_t err = osu_beatmap_create_from_str(&bm, text);
    REQUIRE(err == ERROR_SUCCESS);

    cmania_beatmap_t cbm = {0};
    err = cmania_beatmap_create_from_osu_beatmap(&cbm, &bm);
    REQUIRE(err == ERROR_SUCCESS);
    osu_beatmap_destroy(&bm);

    REQUIRE(cbm.column_count == bm.CS);

    REQUIRE(kv_size(cbm.hitobjects) == kv_size(bm.hitobjects));
    REQUIRE(kv_size(cbm.speed_modifiers) == kv_size(bm.timing_points));

    cmania_beatmap_destroy(&cbm);
}

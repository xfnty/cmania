#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>

#include <filesystem>

#include "error.h"
#include "file.h"
#include "kvec.h"
#include "osu_beatmap.h"


TEST_CASE("Valid Osu Beatmap") {
    const char* text =
    "osu file format v14\n"
    "[General]\n"
    "Mode: 3\n"
    "[Difficulty]\n"
    "CircleSize:  4\n"
    "SliderMultiplier:2.5\n"
    "[TimingPoints]\n"
    "1000,333.33,4,0,0,100,1,0 # BPM 180\n"
    "2000,-50,4,0,0,100,0,0    # BPM 100\n"
    "[HitObjects]\n"
    "256,192,1500,0,0,0:0:0:0:      # COL=2 time=1.5s\n"
    "384,192,2500,128,0,3500:0:0:0: # COL=3 start=2.5s end=3.5s\n"
    ;

    osu_beatmap_t bm = {0};

    err_t err = osu_beatmap_create_from_str(&bm, text);
    REQUIRE(err == ERROR_SUCCESS);

    REQUIRE(bm.CS == 4);
    REQUIRE(bm.SV == 2.5);

    REQUIRE(kv_size(bm.hitobjects) == 2);
    REQUIRE(kv_size(bm.timing_points) == 2);

    REQUIRE(kv_A(bm.hitobjects, 0).start == 1.5);
    REQUIRE(kv_A(bm.hitobjects, 0).end == -1);
    REQUIRE(kv_A(bm.hitobjects, 0).column == 2);
    REQUIRE(kv_A(bm.hitobjects, 1).start == 2.5);
    REQUIRE(kv_A(bm.hitobjects, 1).end == 3.5);
    REQUIRE(kv_A(bm.hitobjects, 1).column == 3);

    REQUIRE(kv_A(bm.timing_points, 0).start == 1);
    REQUIRE(kv_A(bm.timing_points, 0).SV == 2.5);
    REQUIRE(kv_A(bm.timing_points, 0).BPM == 180);
    REQUIRE(kv_A(bm.timing_points, 1).start == 2);
    REQUIRE(kv_A(bm.timing_points, 1).SV == 5);
    REQUIRE(kv_A(bm.timing_points, 1).BPM == 180);

    osu_beatmap_destroy(&bm);
}

TEST_CASE("Real Osu Beatmaps") {
    auto dir = std::filesystem::directory_iterator("./assets/osu/real");
    for (const auto& dentry : dir) {
        if (dentry.is_directory())
            continue;

        INFO("Loading real beatmap: " << dentry.path());
        osu_beatmap_t bm = {0};
        err_t err = osu_beatmap_create_from_file(&bm, dentry.path().c_str());
        REQUIRE(err == ERROR_SUCCESS);
        osu_beatmap_destroy(&bm);
    }
}

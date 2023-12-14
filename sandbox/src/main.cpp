#include <raylib.h>

#include "beatmap.h"


static void raylib_logging_callback(int logLevel, const char *text, va_list args);

int main(int argc, char const *argv[])
{
    if (argc <= 1) {
        PRINTLN(ANSI_COLOR_WHITE "usage: cmania <.osu file>");
        return 0;
    }

    beatmap_t beatmap = {0};
    CHECK_ERROR_LOG_RETURN_VALUE(beatmap_create_from_file(&beatmap, argv[1]), -1, "Could not load \"%s\"", argv[1]);

    beatmap_print(&beatmap);

    InitWindow(640, 480, "Beatmap Playfiled Visualization");
    SetTraceLogCallback(raylib_logging_callback);
    while (!WindowShouldClose()) {
        PollInputEvents();

        ClearBackground(WHITE);
        SwapScreenBuffer();
    }
    CloseWindow();

    beatmap_destroy(&beatmap);
    return 0;
}

void raylib_logging_callback(int logLevel, const char *text, va_list args) {
    if (logLevel < LOG_WARNING)
        return;
}

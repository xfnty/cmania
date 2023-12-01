#ifndef LOGGING_H_
#define LOGGING_H_

#include <stdio.h>
#include <string.h>


#define ANSI_COLOR_RESET          "\033[0;97m"
#define ANSI_COLOR_WHITE          "\033[0;97m"
#define ANSI_COLOR_GRAY           "\033[0;37m"
#define ANSI_COLOR_GREEN          "\033[0;92m"
#define ANSI_COLOR_YELLOW         "\033[0;93m"
#define ANSI_COLOR_RED            "\033[0;91m"
#define ANSI_COLOR_CYAN           "\033[0;96m"
#define ANSI_COLOR_WHITE_BOLD     "\033[1;97m"
#define ANSI_COLOR_GREEN_BOLD     "\033[1;92m"
#define ANSI_COLOR_YELLOW_BOLD    "\033[1;93m"
#define ANSI_COLOR_RED_BOLD       "\033[1;91m"
#define ANSI_COLOR_CYAN_BOLD      "\033[1;96m"

#define __FILENAME__ ((strrchr(__FILE__, '/') != NULL)                      \
     ? strrchr(__FILE__, '/') + 1                                           \
     : (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__))

#define PRINT(fmt, ...)   printf(ANSI_COLOR_GRAY fmt ANSI_COLOR_RESET, ##__VA_ARGS__)
#define PRINTLN(fmt, ...) printf(ANSI_COLOR_GRAY fmt ANSI_COLOR_RESET "\n", ##__VA_ARGS__)

#ifdef SCOPE_NAME  // Should be defined at the very beginning of the file
    #define LOG(fmt, ...) printf(ANSI_COLOR_WHITE_BOLD SCOPE_NAME ": " ANSI_COLOR_RESET fmt ANSI_COLOR_RESET "\n", ##__VA_ARGS__)
#else
    #define LOG(fmt, ...) printf(ANSI_COLOR_RESET fmt ANSI_COLOR_RESET "\n", ##__VA_ARGS__)
#endif
#define __LOG_LEVELNAME_BOILERPLATE(c, s, f, ...) LOG(c s ANSI_COLOR_RESET ": " f, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...)       __LOG_LEVELNAME_BOILERPLATE(ANSI_COLOR_CYAN_BOLD, "debug", fmt, ##__VA_ARGS__)
#define LOG_SUCCESS(fmt, ...)     __LOG_LEVELNAME_BOILERPLATE(ANSI_COLOR_GREEN_BOLD, "success", fmt, ##__VA_ARGS__)
#define LOG_WARNING(fmt, ...)     __LOG_LEVELNAME_BOILERPLATE(ANSI_COLOR_YELLOW_BOLD, "warning", fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...)       __LOG_LEVELNAME_BOILERPLATE(ANSI_COLOR_RED_BOLD, "error", fmt, ##__VA_ARGS__)
#define LOG_FATAL_ERROR(fmt, ...) __LOG_LEVELNAME_BOILERPLATE(ANSI_COLOR_RED_BOLD, "fatal error", fmt, ##__VA_ARGS__)

void logging_init();
void logging_shutdown();


#endif

#ifndef ERROR_H_
#define ERROR_H_

#include <stdlib.h>

#include "util.h"
#include "types.h"
#include "logging.h"


typedef enum {
    ERROR_SUCCESS,
    ERROR_UNDEFINED,
    /* IO */
    ERROR_FILE_NOT_FOUND,
} error_t;

// FIXME: fix this static bs
static const char* ERROR_MESSAGES[] = {
    [ERROR_SUCCESS]         = "OK",
    [ERROR_UNDEFINED]       = "Undefined error",
    [ERROR_FILE_NOT_FOUND]  = "File not found",
};

static inline const char* error_get_message(error_t err) {
    return ERROR_MESSAGES[IS_OUT_OF_BOUNDS(err, ERROR_MESSAGES) ? ERROR_UNDEFINED : err];
}

#define ASSERT(cond)                                    do { if (!(cond)) { LOG_FATAL_ERROR("assertion " ANSI_COLOR_WHITE_BOLD "`%s`" ANSI_COLOR_WHITE " failed" ANSI_COLOR_GRAY " at %s:%d:%s()", #cond, __FILENAME__, __LINE__, __FUNCTION__); exit(-1); } } while(0);
#define ASSERT_RETURN(cond)                             do { if (!(cond)) return; } while(0);
#define ASSERT_RETURN_VALUE(cond, value)                do { if (!(cond)) return (value); } while(0);
#define __ASSERT_LOG_BOILERPLATE(expr, fmt, end, ...)   do { if (!(expr)) { LOG_ERROR(fmt, ##__VA_ARGS__); end; } } while(0);
#define ASSERT_LOG(expr, fmt, ...)                     __ASSERT_LOG_BOILERPLATE(expr, fmt, exit(err), ##__VA_ARGS__)
#define ASSERT_LOG_RETURN(expr, fmt, ...)              __ASSERT_LOG_BOILERPLATE(expr, fmt, return, ##__VA_ARGS__)
#define ASSERT_LOG_RETURN_VALUE(expr, value, fmt, ...) __ASSERT_LOG_BOILERPLATE(expr, fmt, return value, ##__VA_ARGS__)
#define ASSERT_LOG_PROPAGATE(expr, fmt, ...)           __ASSERT_LOG_BOILERPLATE(expr, fmt, return err, ##__VA_ARGS__)

#define CHECK_ERROR(expr)                       do { error_t err = (expr); if (err != ERROR_SUCCESS) { LOG_FATAL_ERROR("expression " ANSI_COLOR_WHITE_BOLD "`%s`" ANSI_COLOR_WHITE " returned an error " ANSI_COLOR_GRAY "(%s) [%d] at %s:%d:%s()", #expr, error_get_message(err), err, __FILENAME__, __LINE__, __FUNCTION__); exit(-1); } } while(0);
#define CHECK_ERROR_RETURN(expr)                do { error_t err = (expr); if (err != ERROR_SUCCESS) return; } while(0);
#define CHECK_ERROR_RETURN_VALUE(expr)          do { error_t err = (expr); if (err != ERROR_SUCCESS) return (value); } while(0);
#define CHECK_ERROR_PROPAGATE(expr)             do { error_t err = (expr); if (err != ERROR_SUCCESS) return err; } while(0);
#define __CHECK_ERROR_LOG_BOILERPLATE(expr, fmt, end, ...)                           \
    do {                                                                                        \
        error_t err = (expr);                                                                   \
        if (err != ERROR_SUCCESS) {                                                             \
            LOG_ERROR(                                                                           \
                fmt " " ANSI_COLOR_GRAY "(%s) [%d]",                                                  \
                ##__VA_ARGS__,                                                                    \
                error_get_message(err),                                                         \
                err                                                                             \
            );                                                                                  \
            end;                                                                                \
        }                                                                                       \
    } while(0);
#define CHECK_ERROR_LOG(expr, fmt, ...)                      __CHECK_ERROR_LOG_BOILERPLATE(expr, fmt, exit(err), ##__VA_ARGS__)
#define CHECK_ERROR_LOG_RETURN(expr, fmt, ...)               __CHECK_ERROR_LOG_BOILERPLATE(expr, fmt, return, ##__VA_ARGS__)
#define CHECK_ERROR_LOG_RETURN_VALUE(expr, value, fmt, ...)  __CHECK_ERROR_LOG_BOILERPLATE(expr, fmt, return value, ##__VA_ARGS__)
#define CHECK_ERROR_LOG_PROPAGATE(expr, fmt, ...)            __CHECK_ERROR_LOG_BOILERPLATE(expr, fmt, return err, ##__VA_ARGS__)


#endif

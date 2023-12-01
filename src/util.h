#ifndef UTIL_H_
#define UTIL_H_


#define LENGTH(array) (sizeof(array)/sizeof(array[0]))
#define IS_OUT_OF_BOUNDS(i, array) (i < 0 || i >= LENGTH(array))
#define CONSTRAIN(v, a, b) ((v < a) ? a : ((v > b) ? b : v))
#define WRAP(v, a, b) ((v < a) ? b : ((v > b) ? a : v))
#define MIN(a, b) ((a < b) ? a : b)
#define MAX(a, b) ((a > b) ? a : b)


#endif

#ifndef _DEBUG_CONFIG_H_
#define _DEBUG_CONFIG_H_

#include <stdint.h>

#define PRINT_DEBUG 0
extern int debug_flag;

#define debug_print_error(s...) {\
    printf("\e[1;31m[%s:%d %s()]:\e[0m ", __FILE__, __LINE__, __FUNCTION__);\
    printf(s);\
    fflush(stdout);\
}\

#define debug_print_info(s...) {\
    if (PRINT_DEBUG || debug_flag){\
        printf("\e[1;32m[%s:%d %s()]:\e[0m ", __FILE__, __LINE__, __FUNCTION__);\
        printf(s);\
        fflush(stdout);\
    }\
}

#define debug_print_warn(s...) {\
    printf("\e[1;33m[%s:%d %s()]:\e[0m ", __FILE__, __LINE__, __FUNCTION__);\
    printf(s);\
    fflush(stdout);\
}
#endif

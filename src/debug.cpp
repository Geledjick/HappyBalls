#pragma once

#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINTF(s, ...) printf("DEBUG - " s, __VA_ARGS__)
#define DEBUG_PUTS(s) puts("DEBUG - " s)
#else
#define DEBUG_PRINTF(s, ...)
#define DEBUG_PUTS(s) 
#endif // DEBUG
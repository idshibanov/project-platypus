// Project Platypus
// debug.h - debug print statement

#ifndef PLA_DEBUG_H
#define PLA_DEBUG_H

#ifndef NDEBUG
#include <stdio.h>
#endif

void prn_debug(const char* str)
{
#ifndef NDEBUG
printf("%s\n", str);
#endif
}

#endif /* PLA_DEBUG_H */

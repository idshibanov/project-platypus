// Project Platypus
// bitmath.h - binary math functions

#ifndef PLA_BITMATH_H
#define PLA_BITMATH_H

#include "datatypes.h"

//  function GB - GetByte(source, starting_bit)

template <typename T>
static inline uchar GB(const T x, const uchar s)
{
    static uchar n = 8; // get 8 bits
    return (x >> s) & (((T)1U << n) - 1);
}

#endif /* PLA_BITMATH_H */



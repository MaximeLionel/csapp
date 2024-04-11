#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <math.h>

/* Access bit-level representation floating-point number */
typedef unsigned float_bits;

float u2f(unsigned x)
{
    unsigned* p_x = &x;
    return *(float*)p_x;
}

unsigned f2u(float f)
{
    return *(unsigned*)&f;
}

/* Compute -f.  If f is NaN, then return f. */
float_bits float_half(float_bits f)
{
    /* Decompose bit representation into parts */
    unsigned sign = f >> 31;  // to align absolute value
    unsigned exp = f >> 23 & 0xFF;
    unsigned frac = f & 0x7FFFFF;

    /* NaN */
    if (exp == 0xFF && frac != 0) return f;

    int carrybit = (frac & 0x3) == 0x3; // When do round-to-even, if or not need to carry bit

    /* Denormalized value */
    if (exp == 0)
    {
        frac = frac >> 1;
        frac += carrybit;
    }
    /* Normalized transit to denormalized */
    else if (exp == 0x1)
    {
        exp = 0;
        frac = ((frac >> 1) | 0x400000) + carrybit;
    }
    else exp -= 1;

    /* Reassemble bits */
    return (sign << 31) | (exp << 23) | frac;
}

int main()
{
    unsigned x = 0;
    while (x < UINT_MAX)
    {
        assert((u2f(x)) / 2 == u2f(float_half(x)));
        x++;
        printf("0x%X ", x);
    }
    return 0;
}

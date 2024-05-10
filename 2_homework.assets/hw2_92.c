#include <stdio.h>
#include <limits.h>
#include <assert.h>

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
float_bits float_negate(float_bits f)
{
    /* Decompose bit representation into parts */
    unsigned sign = f >> 31;
    unsigned exp = f >> 23 & 0xFF;
    unsigned frac = f & 0x7FFFFF;

    /* NaN */
    if (exp == 0xFF && frac != 0) return f;

    /* Reassemble bits */
    return (~sign << 31) | (exp << 23) | frac;
}

int main()
{
    unsigned x = 0;
    while (x <= UINT_MAX)
    {
        assert(-u2f(x) == u2f(float_negate(x)));
        x++;
        printf("0x%08X \n", x);
    }
    return 0;
}

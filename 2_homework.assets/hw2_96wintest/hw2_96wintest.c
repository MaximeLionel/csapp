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

/*
 * Compute (int) f.
 * If conversion causes overflow or f is NaN, return 0x80000000
 */
int float_f2i(float_bits f)
{
    /* Decompose bit representation into parts */
    unsigned sign = f >> 31;  // to align absolute value
    unsigned exp = f >> 23 & 0xFF;
    unsigned frac = f & 0x7FFFFF;
    unsigned result = 0;
    int E;
    int bias = 0x7F;
    unsigned M;

    if (exp == 0xFF && frac != 0) return 0x80000000;

    
    if (exp < bias && exp >= 0)    // value from -1 to +1 (all denormalized values and part of normalized values)
    {
        result = 0;
        return result;
    }
    else if (exp >= bias + 31)      //  value from INT_MIN/INT_MAX to negative infinity or positive infinity:
    {
        result = 0x80000000;
        return result;
    }
    else // (e > bias && e < bias + 31): value from INT_MIN to -1 and from +1 to INT_MAX (Normalized values)
    {
        E = exp - bias;
        M = 0x800000 | frac; // this M already shift n=23 bits
        //result = M >> 23 << E = M << E-23;
        if (E > 23) result = M << E - 23;
        else result = M >> 23 - E;
    }


    /* Reassemble bits */
    return (sign << 31) | result;
}

int main()
{
    unsigned x = 0x3F800000;// 0 01111111 00000000000000000000000
    while (x < UINT_MAX)
    {
        float fx = *(float*) & x;
        int ix = float_f2i(x);
        assert((int)(fx) == ix);
        x++;
        printf("Original unsigned: 0x%08X,  Integer: %08d, Floating: %08f\n", x, ix, fx);
    }
    return 0;
}


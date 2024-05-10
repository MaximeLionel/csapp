
#include <limits.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>


float u2f(unsigned x)
{
    unsigned* p_x = &x;
    return *(float*)p_x;
}

float fpwr2(int x)
{

    /* Result exponent and fraction */
    unsigned exp, frac;
    unsigned u;

    /*if x < smallest positive denormalized, return 0.0 (denormalized) */
    if (x < -149) {
        /* Too small. Return 0.0 */ 
        exp = 0;
        frac = 0;
    } 
    /*if x < smallest positive normalized / largest positive denormalized, return (denormalized)*/
    else if(x < -126) 
    {   /* Denormalized result */ 
        exp = 0;
        frac = 1 << (x + 149);
    }
    else if (x < 128) 
    { /* Normalized result. */ 
        exp = x + 127;
        frac = 0;
    }
    else {
        /* Too big. Return +oo */ 
        exp = 0xFF;
        frac = 0;
    }

    /* Pack exp and frac into 32 bits */
    u = (exp << 23 | frac) & INT_MAX;
    /* Return as float */
    return u2f(u);

}

int main()
{
    assert(fpwr2(0) == powf(2, 0));
    assert(fpwr2(100) == powf(2, 100));
    assert(fpwr2(-100) == powf(2, -100));
    assert(fpwr2(10000) == powf(2, 10000));
    assert(fpwr2(-10000) == powf(2, -10000));
    return 0;
}


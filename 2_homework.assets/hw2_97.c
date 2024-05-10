#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <math.h>

/* Access bit-level representation floating-point number */
typedef unsigned float_bits;

unsigned bit_width(unsigned ux)
{
    if ((ux & INT_MIN) != 0)
    {
        return 0x20;
    }

    unsigned width = 0;
    while (ux != 0)
    {
        width++;
        ux = ux >> 1;
    }
    return width;
}

unsigned bit_mask(unsigned x)
{
    unsigned w = sizeof(unsigned) << 3;
    if (x == 0) return 0;
    return (unsigned)(-1) >> (w - x);
}

float u2f(unsigned x)
{
    unsigned* p_x = &x;
    return *(float*)p_x;
}

unsigned f2u(float f)
{
    return *(unsigned*)&f;
}

/* Generate fraction bits based on the u.*/
/* For example, if u = 0b 11, then the function will return 0b 1100 0000 ... 0000*/
unsigned frac_gen(unsigned u)
{
    unsigned ret = u;
    while ((0x80000000 & ret) == 0)
    {
        ret = ret << 1;
    }
    return ret;
}

/* Compute (float) i */
float_bits float_i2f(int i)
{
    /* Decompose bit representation into parts */
    unsigned sign;
    unsigned frac;
    unsigned exp;

    /* Other useful local variables */
    unsigned width;     // bit width of integer i
    unsigned u;         // usigned representation of integer i
    int bias = 0x7F;    // k = 8
    unsigned frac_mask; // frac = frac_mask & u
    unsigned frac_width;// the width of frac
    unsigned raw_frac;// fraction on the rightmost.
    unsigned rounding_part, rounding_mask, rounding_halfflag;
    float_bits result;

    if ((INT_MIN & i) == 0)
    {
        sign = 0;
        u = (unsigned)i;
    }
    else {
        sign = 1;
        u = (unsigned)(~i + 1);
    }

    // exception: i == 0
    if (i == 0) {
        frac = 0;
        exp = 0;
        return sign << 31 | exp << 23 | frac;
    }
    // exception; i == INT_MIN
    else if (i == INT_MIN) {
        sign = 1;
        exp = bias + 31;
        frac = 0;
        return sign << 31 | exp << 23 | frac;
    }

    width = bit_width(u);
    frac_width = width - 1;
    frac_mask = bit_mask(frac_width);
    raw_frac = u & frac_mask;

    if (frac_width < 24) // n = 23 is the precision of single-floating representation. No rounding required.
    {  
        exp = frac_width + bias;
        frac = raw_frac << (23 - frac_width);
    }
    else    // consider rounding-to-even rule
    {
        exp = frac_width + bias;
        rounding_mask = bit_mask(frac_width - 23);
        rounding_part = rounding_mask & raw_frac;
        rounding_halfflag = (rounding_mask + 1) >> 1;
        if (rounding_part > rounding_halfflag)  // round to closest 
        {

            // frac = (frac + rounding_halfflag) & (~rounding_mask);
            raw_frac = raw_frac + rounding_halfflag;
            if (bit_width(raw_frac) > frac_width)     // if(there's carry bit) exp need to add
            {
                exp++;
                //rounding_mask = (rounding_mask << 1) + 1;
                raw_frac = raw_frac & (~rounding_mask);
                //frac_width++;
            }
            
        }
        else if (rounding_part < rounding_halfflag) // round to closest
        {
            raw_frac = raw_frac & (~rounding_mask);
        }
        else // round to even
        {
            unsigned odd_bit = raw_frac & (~rounding_mask) & ((rounding_mask << 1) + 1);
            if (odd_bit)
            {
                raw_frac = raw_frac + rounding_part;
                if (bit_width(raw_frac) > frac_width)     // if(there's carry bit) exp need to add
                {
                    exp++;
                    //rounding_mask = (rounding_mask << 1) + 1;
                    raw_frac = raw_frac & (~rounding_mask);
                    //frac_width++;
                }
            }
            else
            {
                raw_frac = raw_frac & (~rounding_mask);
            }
        }
        frac = raw_frac >> (frac_width - 23);
        frac = frac & 0x7FFFFF;
    }

    return sign << 31 | exp << 23 | frac;
}

int main()
{
    //int ix = INT_MIN + 1;// 0 01111111 00000000000000000000000
    //int ix = INT_MIN+1;
    //int ix = 0;
    int ix = INT_MAX - 0x100;
    while (ix < INT_MAX)
    {
        float_bits fbx = float_i2f(ix);
        float fx = *(float*)&fbx;
        float fix = (float)ix;
        assert(fix == fx);
        printf("ix value: %08d, Floating from float_i2f: %08f, Floating from casting: %08f\n",ix, fx, fix);
        ix++;
    }
    return 0;
}


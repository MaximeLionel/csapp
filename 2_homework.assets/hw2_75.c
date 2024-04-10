
#include <stdio.h>
#include <inttypes.h>

int signed_high_prod(int x, int y)
{
    int64_t res = (int64_t) x * y;
    res = res >> 32;
    return res;
}

unsigned unsigned_high_prod(unsigned x, unsigned y)
{
    // $=signed\_high\_prod(x,y) + x\cdot y_{w-1}+y\cdot x_{w-1}+y_{w-1}\cdot x_{w-1}2^w$
    unsigned y31 = y >> 31;
    unsigned x31 = x >> 31;
    return signed_high_prod(x, y) + x * y31 + y * x31;
}

int main()
{
    unsigned x = 0xFFFFFFFF;
    unsigned y = 0x10;
    printf("unsigned_high_prod returns 0x%X with 0x%X + 0x%X\n", unsigned_high_prod(x,y),x,y);

    return 0;
}


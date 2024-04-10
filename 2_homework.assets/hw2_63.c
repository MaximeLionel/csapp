#include <stdio.h>

/* Perform right shift logically */
unsigned srl(unsigned x, int k) {
    unsigned xsra = (int)x >> k;  // right shift arithmetically
    unsigned w = sizeof(int) * 8; // bit width of integer
    unsigned mask = ~(0xFFFFFFFF << (w-k));
    return xsra & mask;
}

/* Perform right shift arithmetically */
int sra(int x, int k) {
    int xsrl = (unsigned)x >> k; // right shift arithmetically
    unsigned w = sizeof(int) * 8; // bit width of integer
    unsigned mask = 0xFFFFFFFF << (w - k);
    return xsrl | mask;

}

int main()
{
    unsigned test1 = 0x80000000;
    printf("0x80000000 right shift logically for 3 bits is: 0x %X\n", srl(test1, 3));
    printf("0x80000000 right shift logically for 8 bits is: 0x %X\n", srl(test1, 8));
    printf("0x80000000 right shift logically for 16 bits is: 0x %X\n", srl(test1, 16));
    printf("\n");
    printf("0x80000000 right shift arithmetically for 3 bits is: 0x %X\n", sra(test1, 3));
    printf("0x80000000 right shift arithmetically for 8 bits is: 0x %X\n", sra(test1, 8));
    printf("0x80000000 right shift arithmetically for 16 bits is: 0x %X\n", sra(test1, 16));
    return 0;
}

#include<stdio.h>

/* Declaration of data type where 4 bytes are packed
   into an unsigned */
typedef unsigned packed_t;

/* Failed attempt at xbyte */
int xbyte(packed_t word, int bytenum)
{
    return (word >> (bytenum << 3)) & 0xFF;
}

/* Correct xbyte*/
int correct_xbyte(packed_t word, int bytenum)   // suppose word = 0xFFF80000 bytenum = 2
{
    int w = sizeof(int) << 3;       // w = 32
    int temp_shift = word << (w - (bytenum + 1) << 3);  // temp_shift = 0xF8000000
    int result = temp_shift >> (w - 1 << 3);            // result = 0xFFFFFFF8
    return result;
}

int main()
{
    packed_t packedword = -10;
    int bytenum = 0;
    printf("xbytes: extract bytenumber%d from word 0x%08X.  Return 0x%08X as signed integer.\n",bytenum,(int)packedword, xbyte(packedword,bytenum));
    printf("correct_xbyte: extract bytenumber%d from word 0x%08X.  Return 0x%08X as signed integer.\n", bytenum, (int)packedword, correct_xbyte(packedword, bytenum));
    packedword = 0xFFF80000;
    bytenum = 2;
    printf("correct_xbyte: extract bytenumber%d from word 0x%08X.  Return 0x%08X as signed integer.\n", bytenum, (int)packedword, correct_xbyte(packedword, bytenum));
    return 0;
}
#include <limits.h>
#include <stdio.h>


/* Divide by power of 2. Assume 0 <= k < w-1 */
int divide_power2(int x, int k)
{
    // $x/2^k=(x<0? x+(1<<k)-1 : x)>>k$
    int sign_bit = ((x & INT_MIN) == INT_MIN);
    sign_bit && (x = x + (1 << k) - 1);
    return x >> k;
}

int main()
{
    int x = 8;
    int k = 3;
    printf("divide_power2(0x%X, 0x%X) returns 0x%X.\n",x,k, divide_power2(x,k));
    x = -17;
    printf("divide_power2(0x%X, 0x%X) returns 0x%X.\n", x, k, divide_power2(x, k));
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

#include <stdio.h>
#include <limits.h>

/* Determine whether arguments can be subtracted without overflow */
int tsub_ok(int x, int y)
{
    /*
   * if x > 0, y < 0 but res <= 0, it's a positive overflow
   * if x < 0, y > 0 but res >= 0, it's a negetive overflow
   */
    int res = x - y;
    int pos_flag = !(INT_MIN & x) && (INT_MIN & y) && ((INT_MIN & res)||(res==0)); // INT_MIN = 0x 8000 0000
    int neg_flag = (INT_MIN & x) && !(INT_MIN & y) && (!(INT_MIN & res) || (res == 0));
    return !(pos_flag || neg_flag);
}

int main()
{
    int x = 5;
    int y = 6;
    printf("tsub_ok() return %d when check %d - %d\n", tsub_ok(x, y), x, y);

    x = INT_MIN;
    printf("tsub_ok() return %d when check %X - %X\n", tsub_ok(x, y), x, y);

    x = INT_MAX;
    y = -5;
    printf("tsub_ok() return %d when check %X - %X\n", tsub_ok(x, y), x, y);
    return 0;
}


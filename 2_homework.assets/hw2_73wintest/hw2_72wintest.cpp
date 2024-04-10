#include<stdio.h>
#include<limits.h>

/* Addition that saturates to TMin or TMax */
int saturating_add(int x, int y)
{
    /*
   * if x > 0, y > 0 but sum < 0, it's a positive overflow
   * if x < 0, y < 0 but sum >= 0, it's a negetive overflow
   */
    int sum = x + y;
    int pos_flag = !(INT_MIN & x) && !(INT_MIN & y) && (INT_MIN & sum); // INT_MIN = 0x 8000 0000
    int neg_flag = (INT_MIN & x) && (INT_MIN & y) && !(INT_MIN & sum);

    pos_flag && (sum = INT_MAX);
    neg_flag && (sum = INT_MIN);

    return sum;
}

int main()
{
    int x = 5;
    int y = 5;
    printf("0x%x saturating_add 0x%x equals 0x%x.\n", x, y, saturating_add(x, y));
    x = INT_MAX;
    printf("0x%x saturating_add 0x%x equals 0x%x.\n", x, y, saturating_add(x, y));
    x = -5;
    y = INT_MIN;
    printf("0x%x saturating_add 0x%x equals 0x%x.\n", x, y, saturating_add(x, y));
    return 0;
}


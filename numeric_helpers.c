#include <math.h>

int 
calc_expo(int x, int n, int y)
{
    int xn, p;

    xn = 1;
    for (p = 1; p <= n; ++p) {
        if (y / xn < x)
            return 1;
        xn *= x;
    }
    if (xn == y)
        return 0;
    return -1;
}

int
is_power(int nr, int expo)
{
    int val, left, right, mid, status;

    if (nr <= 0) /* Check for negative numbers */
        return 0;
    if (nr == 1) /* Check if nr is 1 */
        return 1;
    if (expo == 2) { /* Check if we can use sqrt() */
        val = sqrt(nr);
        if (val * val == nr)
            return 1;
        if ((val + 1) * (val + 1) == nr)
            return 1;
        if ((val - 1) * (val - 1) == nr)
            return 1;
        return 0;
    }

    /* Otherwise, do binary search on base */
    left = 1; 
    right = sqrt(nr) + 2;
    while (left <= right) {
        mid = (left + right) / 2;
        status = calc_expo(mid, expo, nr);

        if (status == 0)
            return 1;
        else if (status < 0)
            left = mid + 1;
        else
            right = mid - 1;
    }

    return 0;
}
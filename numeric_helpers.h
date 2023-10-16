#ifndef NUMERIC_HELPERS_H
#define NUMERIC_HELPERS_H

/* Returns 
    -1 -> x^n < y
     0 -> x^n = y
    +1 -> x^n > y */
int calc_expo(int x, int n, int y);

/* Check if nr is of form base^expo */
int is_power(int nr, int expo);

#endif /* NUMERIC_HELPERS_H */
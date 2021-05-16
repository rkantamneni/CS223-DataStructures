#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "printFixed.h"

// test harness for printFixed

#define BASE (10) // to force base 10 in strtoll

int
main(int argc, char **argv)
{

    if(argc != 5) {
        fprintf(stderr, "Usage: %s number separator decimalPoint precision\n", argv[0]);
        return 1;
    }

    long long number = strtoll(argv[1], 0, BASE);
    char separator = argv[2][0];
    char decimalPoint = argv[3][0];
    int precision = atoi(argv[4]);

    printFixed(number, separator, decimalPoint, precision);
    putchar('\n');

    return 0;
}

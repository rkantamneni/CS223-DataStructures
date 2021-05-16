#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#define MIN_KEY_LETTER ('z')

unsigned long long powerFunction (int x, int h) {
    int i; 
    unsigned long long number = 1;

    for (i = 0; i < h; ++i){
        number *= x;
    }    
    return number;
}



int main(int argc, char **argv) {
    int r = 26;
    int k = 1;
    printf("%llu", powerFunction(26, 7));
}
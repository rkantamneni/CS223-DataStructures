#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "heapsort.h"

void swap(int *x, int *y){
    int temp = *x;
    *x = *y;
    *y = temp;
}

size_t parent(size_t i){
    return (i-1)/2;
}

void floatUp(size_t i, int *a){
    // printf("%d \n", a[parent(i)]);
    // printf("%d \n", a[i]);

    if(i != 0 && a[parent(i)] < a[i]) { //a[2] < a[5]
        swap(&a[parent(i)], &a[i]);
        floatUp(parent(i), a);
    }
}
//floatUp(4, a);
    //     10
    //  7      8
    // 3 9

/* compute child 0 or 1 */
#define Child(x, dir) (2*(x)+1+(dir))

void floatDown (int n, int *a, int pos){
    int x;
    /* save original value once */
    x = a[pos];

    for(;;) {
        if(Child(pos, 1) < n && a[Child(pos, 1)] > a[Child(pos, 0)]) {
            /* maybe swap with Child(pos, 1) */
            if(a[Child(pos, 1)] > x) {
                a[pos] = a[Child(pos, 1)];
                pos = Child(pos, 1);
            } else {
                /* x is bigger than both kids */
                break;
            }
        } else if(Child(pos, 0) < n && a[Child(pos, 0)] > x) {
            /* swap with Child(pos, 0) */
            a[pos] = a[Child(pos, 0)];
            pos = Child(pos, 0);
        } else {
            /* done */
            break;
        }
    }

    a[pos] = x;
}


int main(int argc, char **argv) {
   
    int *a = calloc(8, sizeof(int));


    // a[0] = -7;
    // a[1] = -2;
    // a[2] = -3;
    // a[3] = -5;
    // a[4] = -6;
    // a[5] = -7;
    // a[6] = -8;
    // a[7] = -9;
    a[0] = 1;
    a[1] = 8;
    a[2] = 9;
    a[3] = 6;
    a[4] = 7;
    a[5] = 5;
    a[6] = 4;
    a[7] = 2;

    for(size_t i = 0; i < 8; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");

    //floatUp(7, a);
    floatDown(7, a, 0);

 
    for(size_t i = 0; i < 8; i++) {
        printf("%d ", a[i]);
        //assert(a[i] <= a[i+1]);
    }
    printf("\n");

    return 0;
}
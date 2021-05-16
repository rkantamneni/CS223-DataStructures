#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "orderBook.h"

//Dictionary Struct
struct orderBook { 
    int *pendingBuys;    
    size_t numPendingBuys;
    size_t reallocSizeBuys;

    int *pendingSells;
    size_t numPendingSells;
    size_t reallocSizeSells;
};

// Make a new empty order book.
OrderBook orderBookCreate(void){
    OrderBook o = malloc( sizeof(struct orderBook) );
    o->reallocSizeBuys = 20;
    o->pendingBuys = calloc(o->reallocSizeBuys, sizeof(int));
    o->numPendingBuys = 0;

    o->reallocSizeSells = 20;
    o->pendingSells = calloc(o->reallocSizeSells, sizeof(int));
    o->numPendingSells = 0;
    return o;
}

// Destroy an order book,
// freeing all space
// and discarding all pending orders.
void orderBookDestroy(OrderBook o){
    free(o->pendingBuys);
    free(o->pendingSells);
    free(o);
}






void swap(int *x, int *y){
    int temp = *x;
    *x = *y;
    *y = temp;
}

size_t parent(size_t i){
    return (i-1)/2;
}

void floatUp(size_t i, int *a){

    if(i != 0 && a[parent(i)] < a[i]) { 
        swap(&a[parent(i)], &a[i]);
        floatUp(parent(i), a);
    }
}

/* compute child 0 or 1 */
#define Child(x, dir) (2*(x)+1+(dir))

void floatDown (int n, int *a, int pos){ //n=number of elements, a=array, pos=index we're floating
    int x;
    /* save original value once */
    x = a[pos];

    for(;;) {
        if(Child(pos, 1) < n     &&     a[Child(pos, 1)] > a[Child(pos, 0)]) {
            /* maybe swap with Child(pos, 1) */
            if(a[Child(pos, 1)] > x) {
                a[pos] = a[Child(pos, 1)];
                pos = Child(pos, 1);
            } else {
                /* x is bigger than both kids */
                break;
            }

        } else if(Child(pos, 0) < n    &&    a[Child(pos, 0)] > x) {
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





// Enter a new order in the order book.
//
// If price > 0, it represents a buy order.
// Return value will be price p2 of sell order
// maximizing price + p2 > 0, or 0 if there
// is no such sell order.
//
// If price < 0, it represents a sell order.
// Return value will be price p2 of buy order
// maximizing price + p2 > 0, or 0 if there
// is no such buy order.
//
// In either of the above cases, if 0 is returned,
// then price is entered into the order book
// as a pending order available for future matches.
// If a nonzero value is returned, the corresponding
// pending order is removed.
//
// If price == 0, no order is entered and 
// return value is 0.
int orderBookInsert(OrderBook o, int price){

//***********Incoming Buy order, max heap to find lowest sell order***********    
    if (price > 0){
        int price2 = 0;
        int truth = 0;
        if ((o->numPendingSells > 0)){
            price2 = o->pendingSells[0];
            truth = 1;
        }
        
        if ((price2 + price) > 0 && truth==1){ //Executing Order
                o->pendingSells[0] = o->pendingSells[o->numPendingSells-1];
                o->pendingSells[o->numPendingSells-1] = 0;
                o->numPendingSells--;
                floatDown(o->numPendingSells, o->pendingSells, 0);
                //o->numPendingSells--;
                return price2;
        }
        

        else{
            o->pendingBuys[o->numPendingBuys] = price;
            //o->numPendingBuys++;
            floatUp(o->numPendingBuys, o->pendingBuys);
            o->numPendingBuys++;


            if(o->numPendingBuys == o->reallocSizeBuys){
                o->reallocSizeBuys *= 2;
                o->pendingBuys = realloc(o->pendingBuys, sizeof(int)*o->reallocSizeBuys);
            } 
            return 0;
        }

    }

//***********Incoming Sell order, max heap to find largest buy order***********
    else if (price < 0){
        int price2 = 0;
        int truth = 0;
        if ((o->numPendingBuys > 0)){
            price2 = o->pendingBuys[0];
            truth = 1;
        }
        
        if ((price2 + price) > 0 && truth==1){ //Executing Order
                o->pendingBuys[0] = o->pendingBuys[o->numPendingBuys-1];
                o->pendingBuys[o->numPendingBuys-1] = 0;
                o->numPendingBuys--;
                floatDown(o->numPendingBuys, o->pendingBuys, 0);
                //o->numPendingBuys--;
                return price2;
            
        }

        else{
            o->pendingSells[o->numPendingSells] = price;
            //o->numPendingSells++;
            floatUp(o->numPendingSells, o->pendingSells);
            o->numPendingSells++;


            if(o->numPendingSells == o->reallocSizeSells){
                o->reallocSizeSells *= 2;
                o->pendingSells = realloc(o->pendingSells, sizeof(int)*o->reallocSizeSells);
            } 
            return 0;
        }

    }

//***********Return 0 if price is equal to 0***********
    return 0;
}



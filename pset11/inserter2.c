#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "inserter.h"


// // An Inserter object manages an ordered linked list
// // of null-terminated strings.

struct element {
    struct element *next;  // next element in list
    char *string;          // string stored in this element
};

//typedef struct inserter *Inserter;
struct inserter{
    char *key;
    int height;                /* number of next pointers */
    struct skiplist *next[1];  /* first of many */
};

struct element skiplistCreateNode(char *key, int height){
    element s;

    s = malloc(sizeof(struct skiplist) + sizeof(struct skiplist *) * (height - 1));


    s->key = key;
    s->height = height;

    return s;
}



// Create a new Inserter to manage the list 
// whose head is stored in *head.
// This list should start empty.
Inserter inserterCreate(struct element **head){
    Inserter i = malloc( sizeof(struct inserter) );

    Skiplist s;
    int i;

    /* s is a dummy head element */
    s = skiplistCreateNode(head, MAX_HEIGHT);

    /* this tracks the maximum height of any node */
    s->height = 1;

    for(i = 0; i < MAX_HEIGHT; i++) {
        s->next[i] = 0;
    }

    return s;
    

    return i;
}


// Add a string to the managed list if not
// already present.
// The new string will be a malloc'd copy
// of s.
void inserterAdd(Inserter i, const char *s){
    struct element *e = malloc(sizeof(struct element));
    e->string = strdup(s);

}





// Free all space used by the given Inserter.
// This will *not* free any space used by
// the managed linked list.
void inserterDestroy(Inserter i){
    free(i);

}

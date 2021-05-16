#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <limits.h>
#include <string.h>
#include "inserter.h"

#define NUM_CHILDREN (2)

#define LEFT (0)
#define RIGHT (1)


// struct element {
//     struct element *next;  // next element in list
//     char *string;          // string stored in this element
// };

struct treap { //each is a node
    char *key;
    int heapKey;
    struct treap *child[NUM_CHILDREN];
    struct element *item;

};
typedef struct treap **Treap;

struct inserter{
    struct element **header;
    Treap t;
};

Inserter inserterCreate(struct element **head){
    Inserter i = malloc(sizeof(struct inserter));
    i->t = malloc(sizeof(struct treap*));
    i->header = head;
    return i;
}


// Free all space used by the given Inserter.
// This will *not* free any space used by
// the managed linked list.
void inserterDestroy(Inserter i){
    //Free the treap?
    treapDestroy(i->t);
    free(i);
}

void treapDestroy(struct treap **t) {
    if(*t) {
        for(int dir = LEFT; dir <= RIGHT; dir++) {
            treapDestroy(&(*t)->child[dir]);
        }
    }

    free(*t);
    *t = 0;
}


// return largest element <= key

//Largest node that is less than or equal to the key
// or INT_MIN if there is no such element.
struct element *treapSearchL(struct treap *t, char *key, struct element *insertionPoint){
    if(t == 0) {
        // no key!
        return insertionPoint;

    } else {
        if(strcmp(key, t->key) < 0) {
        // look in left
            return treapSearchL(t->child[LEFT], key,insertionPoint);
        } else {
        // look in right
            return treapSearchL(t->child[RIGHT], key, t->item);
        }
    }
}

// rotate the treap pointed to by parent
// so that child in direction moves up
void treapRotateUp(struct treap **parent, int dir){
    // get pointers to anything that might move
    assert(parent);
    struct treap *child = *parent;
    assert(child);
    struct treap *grandchild = child->child[dir];
    assert(grandchild);
    struct treap *middleSubtreap = grandchild->child[!dir];

    // do the move
    *parent = grandchild;
    grandchild->child[!dir] = child;
    child->child[dir] = middleSubtreap;
}


// insert key into treap pointed to by parent
// if not already present
void treapInsert(struct treap **parent, char *key, struct element *prev, Inserter i) {
    if(*parent == 0) { //Make new node
        // no key!
        *parent = malloc(sizeof(struct treap));
        (*parent)->key = key;
        (*parent)->heapKey = rand();
        (*parent)->child[LEFT] = (*parent)->child[RIGHT] = 0;

        struct element *newNode = malloc(sizeof(struct element));
        (*parent)->item = newNode;

        //Add into linked list
        if (prev == NULL) {
            newNode->next = *i->header; //removed *
            *i->header = newNode;

        } 
        else {
            newNode->next = prev->next;
            prev->next = newNode;
        }

    } else if(strcmp(key, (*parent)->key) == 0) {
        // found it
        return;

    } else if(strcmp(key, (*parent)->key)  < 0) {
        // look in left
        treapInsert(&(*parent)->child[LEFT], key, prev, i);
    } else {
        // look in right
        treapInsert(&(*parent)->child[RIGHT], key, (*parent)->item, i);
    }

    // check heap property
    for(int dir = LEFT; dir <= RIGHT; dir++) {
        if((*parent)->child[dir] != 0 && (*parent)->child[dir]->heapKey > (*parent)->heapKey) {
            treapRotateUp(parent, dir);
        }
    }
}

void inserterAdd(Inserter i, const char *s) {
    //Malloced Copy
    char *s2 = malloc(1 + strlen(s));
    strcpy(s2, s);

    treapInsert(i->t, s2, NULL, i);
}



#define TEST_THRESHOLD (10)

int main(int argc, char **argv) {
    if(argc != 1) {
        fprintf(stderr, "Usage: %s\n", argv[0]);
        return 1;
    }
    return 0;
}

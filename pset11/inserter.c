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
    i->t = calloc(1, sizeof(struct treap*));
    i->header = head;
    return i;
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
// Free all space used by the given Inserter.
// This will *not* free any space used by
// the managed linked list.
void inserterDestroy(Inserter i){
    treapDestroy(i->t);
    free(i->t);
    free(i);
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


//Largest node that is less than or equal to the key
struct element* treapSearchL(struct treap *t, char *key){
    if(t == 0) {
        // no key!
        return NULL;

    } else if(strcmp(key, t->key) < 0) {
        // look in left
        //printf("%s key: %s\n", t->key, key);

        return treapSearchL(t->child[LEFT], key);
    } else {
        //printf("%s key: %s\n", t->key, key);
        // look in right
        struct element *result = treapSearchL(t->child[RIGHT], key);

        if(result == NULL) {
            // didn't find it
            //printf("item: %s\n", t->item->string);
            return t->item;
        } else {
            return result;
        }
    }
}


// insert key into treap pointed to by parent
// if not already present
void treapInsert(struct treap **parent, char *key, struct element *prev) {
    if(*parent == 0) { //Make new node
        // no key!
        *parent = calloc(1, sizeof(struct treap));
        (*parent)->key = key;
        (*parent)->heapKey = rand();
        //(*parent)->child[LEFT] = (*parent)->child[RIGHT] = 0;
        (*parent)->item = prev;

    } else if(strcmp(key, (*parent)->key) == 0) {
        // found it duplicate, do nothing
        return;

    } else if(strcmp(key, (*parent)->key)  < 0) {
        // look in left
        treapInsert(&(*parent)->child[LEFT], key, prev);
    } else {
        // look in right
        treapInsert(&(*parent)->child[RIGHT], key, prev);
    }

    // check heap property
    for(int dir = LEFT; dir <= RIGHT; dir++) {
        if((*parent)->child[dir] != 0 && (*parent)->child[dir]->heapKey > (*parent)->heapKey) {
            treapRotateUp(parent, dir);
        }
    }
}

int treapSearch(const struct treap *t, const char *key){
    if(t == 0) {
        // no key!
        return 0;
    //} else if(key == t->key) {
    } else if(strcmp(key, t->key) == 0) {
        // found it
        return 1;
    //} else if(key < t->key) {
    } else if(strcmp(key, t->key) < 0) {
        // look in left
        return treapSearch(t->child[LEFT], key);
    } else {
        // look in right
        return treapSearch(t->child[RIGHT], key);
    }
}

// static void treapPrintI(struct treap *t, size_t depth) {
//   if (t != NULL) {
//     // print right subtree
//     treapPrintI(t->child[RIGHT], depth+1);
//     for (size_t i = 0; i < depth; i++) {
//       putchar(' '); putchar(' ');
//     }
//     printf("%s (%08x)\n", t->key == NULL ? "NULL" : t->key, t->heapKey);
//     treapPrintI(t->child[LEFT], depth+1);
//   }
// }

void inserterAdd(Inserter i, const char *s) {
    if (treapSearch(*i->t, s) == 1){
        return;
    }
    
    //Malloced Copy
    char *s2 = malloc(1 + strlen(s));
    strcpy(s2, s);

    //Create node
    struct element *node = malloc(sizeof(struct element));
    node->string = s2;

    struct element *insertionPoint = treapSearchL(*(i->t), s2); //largest one smaller than key or null if there is none
    //get root of treap, passing in, treapSearchL not working
    if (insertionPoint == NULL) {
        node->next = *(i->header);
        *(i->header) = node;
    } 
    else {
        node->next = insertionPoint->next;
        insertionPoint->next = node;
    }
    treapInsert(i->t, s2, node);
}

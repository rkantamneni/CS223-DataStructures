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
    //struct element *next;
} typedef **Treap;

struct inserter{
    //struct element **table;
    struct element *header;
    Treap t;
};

Inserter inserterCreate(struct element **head){
    Inserter i = malloc(sizeof(struct inserter));
    i->t = malloc(sizeof(struct treap*));
    i->header = head;
    return i;
}


void inserterAdd(Inserter i, const char *s) {
 
    //Malloced Copy
    char *s2 = malloc(1 + strlen(s));
    strcpy(s2, s);

    //Create node
    struct element *node = malloc(sizeof(struct element));
    node->string = s2;

    struct element *insertionPoint = treapSearchL(i->t, s2);

    if (insertionPoint == NULL) {//when at top of treap
        node->next = *i->header;
        *i->header = node;

    } 
    else {
        node->next = insertionPoint->next;
        insertionPoint->next = node;
    }
    treapInsert(i->t, s2, node);
}





void inserterDestroy(Inserter i){
    //Free the treap?
    free(i);
}



// Invariants: 
// - Every key below child[LEFT] < key < every key below child[RIGHT]
// - Every heapKey in both subtreaps < heapKey.
// heapKeys are chosen randomly to ensure balance with high probability.



void treapDestroy(struct treap **t) {
    if(*t) {
        for(int dir = LEFT; dir <= RIGHT; dir++) {
            treapDestroy(&(*t)->child[dir]);
        }
    }

    free(*t);
    *t = 0;
}

void treapPrintHelper(const struct treap *t, int depth) {
    if(t == 0) {
        return;
    }

    treapPrintHelper(t->child[LEFT], depth+1);

    // print indented root
    for(int i = 0; i < depth; i++) {
        putchar(' ');
    }

    printf("%s [%d]\n", t->key, t->heapKey);

    treapPrintHelper(t->child[RIGHT], depth+1);
}

void treapPrint(const struct treap *t){
    treapPrintHelper(t, 0);
}

//return 1 if it finds key, 0 otherwise
int treapSearch(const struct treap *t, char *key){
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

// return largest element <= key

//Largest node that is less than or equal to the key
// or INT_MIN if there is no such element.
struct element* treapSearchMaxLE(const struct treap *t, char *key){
    if(t == 0) {
        // no key!
        return NULL;
    // } else if(key == t->key) {
    // } else if(strcmp(key, t->key) == 0) {
    //     // found it
    //     return key;
    // //} else if(key < t->key) {
    } else if(strcmp(key, t->key) < 0) {
        // look in left
        return treapSearchMaxLE(t->child[LEFT], key);
    } else {
        // look in right
        struct element *result = treapSearchMaxLE(t->child[RIGHT], key);

        if(result == NULL) {
            // didn't find it
            return t->item;
        } else {
            return result;
        }
    }
}

struct element *treapSearchL(struct treap *t, char *key){
    if(t == 0) {
        // no key!
        return NULL;
    } else if(strcmp(key, t->key) == 0) {
        // found it
        return t->;
    } else if(strcmp(key, t->key) < 0) {
        // look in left
        return treapSearchL(t->child[LEFT], key);
    } else {
        // look in right
        //char *result = treapSearchMaxLE2(t->child[RIGHT], key);

        if(treapSearchL(t->child[RIGHT], key) == NULL) {
            // didn't find it
            //return t->key;
            return NULL;
        } else {
            return t;
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
void treapInsert(struct treap **parent, char *key, struct element *node) {
    if(*parent == 0) {
        // no key!
        *parent = malloc(sizeof(struct treap));
        (*parent)->key = key;
        (*parent)->heapKey = rand();
        (*parent)->child[LEFT] = (*parent)->child[RIGHT] = 0;
    // } else if(key == (*parent)->key) {
    } else if(strcmp(key, (*parent)->key) == 0) {
        // found it
        return;
    //} else if(key < (*parent)->key) {
    } else if(strcmp(key, (*parent)->key)  < 0) {
        // look in left
        treapInsert(&(*parent)->child[LEFT], key);
    } else {
        // look in right
        treapInsert(&(*parent)->child[RIGHT], key);
    }

    // check heap property
    for(int dir = LEFT; dir <= RIGHT; dir++) {
        if((*parent)->child[dir] != 0 && (*parent)->child[dir]->heapKey > (*parent)->heapKey) {
            treapRotateUp(parent, dir);
        }
    }
}



#define TEST_THRESHOLD (10)

int main(int argc, char **argv) {
    if(argc != 1) {
        fprintf(stderr, "Usage: %s\n", argv[0]);
        return 1;
    }

    struct treap *t = 0;
    char *key;
    key = "a";
    //while(scanf("%c", &key) == 1) {
    treapInsert(&t, key);
    treapPrint(t);
    printf("\n");
        //printf("--- largest <= %d is %d\n", TEST_THRESHOLD, treapSearchMaxLE(t, TEST_THRESHOLD));
    //}
    key = "b";
    treapInsert(&t, key);
    treapPrint(t);
    printf("\n");
    key = "c";
    treapInsert(&t, key);
    treapPrint(t);
    printf("\n");

    key = "k";
    treapInsert(&t, key);
    treapPrint(t);
    printf("\n");
    key = "c";
    treapInsert(&t, key);
    treapPrint(t);
    printf("\n");

    key = "ab";
    treapInsert(&t, key);
    treapPrint(t);
    printf("\n");

    //printf("%s", treapSearchMaxLE(t, "g"));
    printf("%d", treapSearch(t, "ab"));



    treapDestroy(&t);

    return 0;
}

// void inserterAdd(Inserter i, const char *s){
//     struct element *e = malloc(sizeof(struct element));
//     e->string = strdup(s); //need to free later

//     if (treapSearch(i->t, e->string) == 0){ //If not found

//         treapInsert(i->t, e->string);

//         struct treap *t2 = treapSearchMaxLE2(i->t, e->string);
//         e->next = t2->next; //Inserting into the main linked list
//         t2->next = e;

//         //i->numOfElemnts++;
//     }

// }
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <string.h>


#define MAX_HEIGHT (2)

struct element {
    struct element *next;  // next element in list
    char *string;          // string stored in this element
};

//typedef struct inserter *Inserter;
struct inserter{
    size_t numOfElemnts;
    struct element **table;
    struct element *top; //go anywhere from the top
};
typedef struct inserter *Inserter;


struct skiplist {
    char *key; 
    int height;                /* number of next pointers */
    struct skiplist *next[1];  /* first of many */
    struct element *below;
};
typedef struct skiplist * Skiplist;

/* choose a height according to a geometric distribution */
int chooseHeight(void) {
    int i;
    for(i = 1; i < MAX_HEIGHT && rand() % 2 == 0; i++); 
    return i;
}

/* create a skiplist node with the given key and height */
/* does not fill in next pointers */
Skiplist skiplistCreateNode(char *key, int height){
    Skiplist s;

    s = malloc(sizeof(struct skiplist) + sizeof(struct skiplist *) * (height - 1));

    s->key = key;
    s->height = height;

    return s;
}

/* create an empty skiplist */
Skiplist skiplistCreate(void){
    Skiplist s;
    int i;

    /* s is a dummy head element */
    s = skiplistCreateNode("a", MAX_HEIGHT);

    /* this tracks the maximum height of any node */
    s->height = 1;

    for(i = 0; i < MAX_HEIGHT; i++) {
        s->next[i] = 0;
    }

    return s;
}

void printList(Skiplist s){
    while(s) {
        printf("%s\n", s->key);
        s = s->next[0];;
    }
}

/* free a skiplist */
void skiplistDestroy(Skiplist s){
    Skiplist next;

    while(s) {
        next = s->next[0];
        free(s);
        s = next;
    }
}

/* return maximum key less than or equal to key */
/* or INT_MIN if there is none */
char* skiplistSearch(Skiplist s, char *key){
    int level;

    for(level = s->height - 1; level >= 0; level--) {
        while(s->next[level] && (strcmp(s->next[level]->key,key)<=0)) {
            s = s->next[level];
        }
    }

    return s->key;
}

/* insert a new key into s */
void skiplistInsert(Skiplist s, char *key){
    int level;
    Skiplist elt;

    if (strcmp(skiplistSearch(s, key), key) != 0){

        elt = skiplistCreateNode(key, chooseHeight());

        if(elt->height > s->height) {
            s->height = elt->height;
        }

        /* search through levels taller than elt */
        for(level = s->height - 1; level >= elt->height; level--) {
            //while(s->next[level] && s->next[level]->key < key) {
            while(s->next[level] && (strcmp(s->next[level]->key,key)<0)) {
                s = s->next[level];
            }
        }

        /* now level is elt->height - 1, we can start inserting */
        for(; level >= 0; level--) {
            //while(s->next[level] && s->next[level]->key < key) {
            while(s->next[level] && (strcmp(s->next[level]->key, key)<0)) {
                s = s->next[level];
                
            }

            /* s is last entry on this level < new element */
            /* do list insert */
            elt->next[level] = s->next[level];
            s->next[level] = elt;

            if (level == 0){ //Insert into linked list at level 0
                printf("%s  %s\n",s->key, elt->key);
                //s->key//a
            }

        }
    }
}


int main(int argc, char **argv){
    Skiplist s;
    int i;


    s = skiplistCreate();

    // Inserter i;
    // i = inserterCreate(struct element **head);
    
    // malloc( sizeof(struct inserter) );
    // i->numOfElemnts = 0;
    // i->table = calloc(i->numOfElemnts, sizeof(struct element *));




    skiplistInsert(s, "sue");
    skiplistInsert(s, "mack");
    skiplistInsert(s, "brad");
    skiplistInsert(s, "zack");
    skiplistInsert(s, "greg");
    skiplistInsert(s, "a");
    skiplistInsert(s, "aaron");
    skiplistInsert(s, "prober");
    skiplistInsert(s, "will");
    skiplistInsert(s, "revant");
    printf("\n");
    printList(s);
    printf("\n");

    //printf("%s", skiplistSearch(s, "f"));

    // assert(skiplistSearch(s, -1) == INT_MIN);

    // for(i = 0; i < n; i += 2) {
    //     assert(skiplistSearch(s, i) == i);
    //     assert(skiplistSearch(s, i+1) == i);
    // }

    // for(i = 0; i < n; i += 4) {
    //     skiplistDelete(s, i);
    // }

    // assert(skiplistSearch(s, 0) == INT_MIN);

    // for(i = 2; i < n; i += 4) {
    //     assert(skiplistSearch(s, i) == i);
    //     assert(skiplistSearch(s, i+1) == i);
    //     assert(skiplistSearch(s, i+2) == i);
    //     assert(skiplistSearch(s, i+3) == i);
    // }

    // /* make sure insert/delete works with duplicates */
    // for(i = 0; i < n; i++) {
    //     skiplistInsert(s, 0);
    //     assert(skiplistSearch(s, 0) == 0);
    //     assert(skiplistSearch(s, 1) == 0);
    // }

    // for(i = 0; i < n; i++) {
    //     assert(skiplistSearch(s, 0) == 0);
    //     assert(skiplistSearch(s, 1) == 0);
    //     skiplistDelete(s, 0);
    // }

    // assert(skiplistSearch(s, 0) == INT_MIN);

    skiplistDestroy(s);

    return 0;
}
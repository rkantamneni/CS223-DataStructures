#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <inttypes.h>
#include "decrypt3.h"
#define INITIAL_SIZE (1024)
#define SIZE_MULTIPLIER (2)
#define MAX_LOAD_FACTOR (1)
#define FNV_PRIME_64 ((1ULL<<40)+(1<<8)+0xb3)
#define FNV_OFFSET_BASIS_64 (14695981039346656037ULL)


//Dictionary Struct
struct dictStringInt {
    size_t m;    // number of locations in table
    size_t n;    // number of stored elements
    struct elt **table;   // table of linked lists
};

//Elements in Dictionary
struct elt {
    struct elt *next;
    char *value;   // value being stored
    char *key;  // char?, null-terminated character string
};

//Create Dictionary
DictStringInt dictStringIntCreateInternal(size_t initialSize) {
    DictStringInt d = malloc( sizeof(struct dictStringInt) );
    assert(d != 0);
    d->m = initialSize;
    d->n = 0;
    d->table = calloc(d->m, sizeof(struct elt *));

    return d;
}

DictStringInt DictCreate(void) {
    return dictStringIntCreateInternal(INITIAL_SIZE);
}

//Destroy Dictionary
void DictDestroy(DictStringInt d) {
    // walk through all table entries and delete them
    int i;
    struct elt *e;
    struct elt *next;
    for(i = 0; i < d->m; i++) {
        for(e = d->table[i]; e != 0; e = next) {
            next = e->next;

            free(e->key);
            free(e->value);
            free(e);
        }
    }
    free(d->table);
    free(d);
}


//Hashing
uint64_t FNV1a(char *s) {
    uint64_t h = FNV_OFFSET_BASIS_64;
    while(*s != '\0') {
        h ^= *s++;
        h *= FNV_PRIME_64;
    }
    return h;
}


//Grow dictionary when m <= n
void dictGrow(DictStringInt d){
    DictStringInt d2;       /* new dictionary we'll create */
    struct dictStringInt swap;   /* temporary structure for brain transplant */
    int i;
    struct elt *e;

    d2 = dictStringIntCreateInternal(d->m * SIZE_MULTIPLIER);

    for(i = 0; i < d->m; i++) {
        for(e = d->table[i]; e != 0; e = e->next) {
            dictInsert(d2, e->value, e->key); /* note: this recopies everything */
        }
    }

    /* We'll swap the guts of d and d2 */
    swap = *d;
    *d = *d2;
    *d2 = swap;
    DictDestroy(d2); /* then call DictDestroy on d2 */
}

//Insert key-value pair into dictionary
void dictInsert(DictStringInt d, char *value, char *key){
    struct elt *e = malloc(sizeof(struct elt));
    uint64_t h = FNV1a(key);

    //strdup has built-in malloc, needs to be freed in destroy
    e->key = h; //make copy of uint hash key
    e->value = strdup(value); //make copy of char value

    e->next = d->table[h];
    d->table[h] = e;

    d->n++;

    /* grow table if there is not enough room */
    if(d->n >= (d->m * MAX_LOAD_FACTOR)) {
        dictGrow(d);
    }
}

char *DictSearch(DictStringInt d, char *key) {
    struct elt *e;
    //for(e = d->table[FNV1a(key)]; e != 0; e = e->next) {
    for(e = d->table[&key]; e != 0; e = e->next) {
        //if(!strcmp(e->key, key)) { //Not equal to zero
        if(e->key == key) {
            return e->value;
        }
    }
    return 0;
}

//Pre-append salt to the password
char *connectSalt(char *s1, char *s2 , size_t length) { //Online
    char *result = malloc(length + 1); // +1 for the null-terminator
    strcpy(result, s1);
    strcat(result, s2); //Look at this
    return result;
}


int main(int argc, char **argv) {
    //Read in password dictionary, concat with salt, hash, put in dictionary
    DictStringInt d = dictStringIntCreateInternal(INITIAL_SIZE);

    char *b = NULL;
    FILE *inFile = fopen(argv[2], "r");
    size_t zero = 0;
    uint64_t saltHash = precomputeSalt(argv[1]);

    char *c = NULL;
    while ((c = getline(&b, &zero, inFile)) != -1) { //b=password
        char *b = 
        //uint64_t saltHash = FNV1a(s);
        dictInsert(d, b, s); //What do i send into as key
        free(s);
    }

    //Read in encrpyted password until ":"
    //Then, compare hash with hash table
    int c;
    while ((c = getchar()) != EOF){
        putchar(c);
        if(c == ':') { //Getting hashed from encrypted file
            //char *password = getLine();
            uint64_t hash;
            scanf("%" SCNx64 "\n", &hash);
            char *returnVal = DictSearch(d, &hash);
            if (returnVal != 0) {
                printf("%s\n", returnVal);
            }
        }

    }

    //Free something from first part of main
    DictDestroy(d);
    return 0;

}

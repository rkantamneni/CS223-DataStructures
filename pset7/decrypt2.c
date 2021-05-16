#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <inttypes.h>
//Plan
//1. Read inputs from the dictionary, hash them, and put into hash table
//      -Seperate word from colon
//2. Go through hash table and match hash with ones in encrpyted file
//3. Print out password
//insert (check whether m is greater than n), delete, lookup



//Dictionary Struct
struct dictStringInt {
    size_t m;    // number of locations in table
    size_t n;    // number of stored elements
    struct elt **table;   // table of linked lists
};
typedef struct dictStringInt *DictStringInt;

//Elements in Dictionary
struct elt {
    struct elt *next;
    int *value;   // value being stored
    char *key;  // null-terminated character string
};

//Create Dictionary
#define INITIAL_SIZE (1024)
#define SIZE_MULTIPLIER (2)
#define MAX_LOAD_FACTOR (1)
DictStringInt dictStringIntCreateInternal(size_t initialSize) {
    DictStringInt d = malloc(sizeof(struct dictStringInt));

    d->m = initialSize;
    d->n = 0;
    d->table = calloc(d->m, sizeof(struct elt *));

    return d;
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
#define FNV_PRIME_64 ((1ULL<<40)+(1<<8)+0xb3)
#define FNV_OFFSET_BASIS_64 (14695981039346656037ULL)

uint64_t FNV1a(size_t n, const unsigned char *bytes) {
    uint64_t h = FNV_OFFSET_BASIS_64;
    for(size_t i = 0; i < n; i++) {
        h ^= bytes[i];
        h *= FNV_PRIME_64;
    }
    return h;
}

//Grow dictionary when m <= n
void dictGrow(DictStringInt d){
    DictStringInt d2;           /* new dictionary we'll create */
    struct dictStringInt swap;   /* temporary structure for brain transplant */
    int i;
    struct elt *e;

    d2 = dictStringIntCreateInternal(d->m * SIZE_MULTIPLIER);

    for(i = 0; i < d->m; i++) {
        for(e = d->table[i]; e != 0; e = e->next) {
            dictInsert(d2, e->key, e->value); /* note: this recopies everything */
            /* a more efficient implementation would
             * patch out the strdups inside DictInsert
             * to avoid this problem */
        }
    }

    /* We'll swap the guts of d and d2 */
    swap = *d;
    *d = *d2;
    *d2 = swap;
    DictDestroy(d2); /* then call DictDestroy on d2 */
}

//Insert hash into dictionary
void dictInsert(DictStringInt d, char *value, uint64_t *key){
    struct elt *e = malloc(sizeof(struct elt));
    unsigned long h;
    //strdup has built-in malloc, needs to be freed in destroy
    e->key = strdup(key);
    e->value = strdup(value);
    h = hash_function(key) % d->m;

    e->next = d->table[h];
    d->table[h] = e;

    d->n++;

    /* grow table if there is not enough room */
    if(d->n >= (d->m * MAX_LOAD_FACTOR)) {
        dictGrow(d);
    }
}

char *DictSearch(DictStringInt d, uint64_t *key) {
    struct elt *e;

    for(e = d->table[hash_function(key) % d->m]; e != 0; e = e->next) {
        if(!strcmp(e->key, key)) {
            /* got it */
            return e->value;
        }
    }
    return 0;
}

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
    char *saltLength = strlen(argv[1]);

    size_t lineLength = 0;
    while ( (lineLength = getline(&b, &zero, inFile)) != -1) { //b=password
        size_t length = saltLength + lineLength;
        char *s = connectSalt(argv[1], b, length);
        uint64_t saltHash = FNV1a(length, s);
        dictInsert(d, b, saltHash);
        free(s); //Can I move this out of while loop?
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
            if (returnVal != 0){
                printf("%s\n", returnVal);
            }
        }

    }
    DictDestroy(d);
}



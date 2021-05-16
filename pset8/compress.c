#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <inttypes.h>
#include "compress.h"
#define INITIAL_SIZE (256*256)
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
    unsigned char *value;   // bigram being stored
    size_t index; //Index where bigram is stored in bgArray
    uint64_t key;
    int compressChar; //something like 0x80
};

//Elements in Array with biGram and frequency
struct arrayElt {
    unsigned char *bigram;   // value being stored, ba
    int count; //how many times this value has come up from input
};

//Create Dictionary
DictStringInt dictStringIntCreateInternal(size_t initialSize) {
    DictStringInt d = malloc(sizeof(struct dictStringInt) );
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
            //free(e->key);
            free(e->value);
            free(e);
        }
    }
    free(d->table);
    free(d);
}
void ArrayDestory(struct arrayElt **array, size_t arrayIndex){
    for (size_t i = 0; i < arrayIndex; i++) {
        free(array[i]->bigram);
        free(array[i]); //Free null character also?
    }
    free(array);
}

//Hashing for two character
uint64_t FNV1a(unsigned char *s) {

    uint64_t h = FNV_OFFSET_BASIS_64;
    // while(*s != '\0') {
    //     // h ^= *s++;
    //     // h *= FNV_PRIME_64;
    // }
    h ^= *s++;
    h *= FNV_PRIME_64;
    h ^= *s++;
    h *= FNV_PRIME_64;
    return h;
}

//Insert key-value pair into dictionary
void dictInsert(DictStringInt d, unsigned char *value, uint64_t key, size_t index){
    struct elt *e = malloc(sizeof(struct elt));

    e->key = key;
    //e->value = strdup(value); //make copy of char value, need to free
    unsigned char *value2 = malloc(2 * sizeof(char));
    memcpy(value2, value, 2);
    e->value = value2;

    unsigned long long h = key % d->m;

    e->next = d->table[h];
    e->compressChar = 0; //UnNeeded
    e->index = index;

    d->table[h] = e;
    d->n++;
}
//Used for second hashing
void dictInsert2(DictStringInt d2, unsigned char *value, uint64_t key, int compress){
    struct elt *e = malloc(sizeof(struct elt));

    e->key = key;
    //e->value = strdup(value); //make copy of char value, need to free
    unsigned char *value2 = malloc(2 * sizeof(char));
    memcpy(value2, value, 2);
    e->value = value2;

    unsigned long long h = key % d2->m;

    e->next = d2->table[h];
    e->compressChar = compress;
    e->index = 1; //UnNeeded

    d2->table[h] = e;
    d2->n++;
}

//Sets pointer index to bigram's index, comes from hash table
int indexFind(DictStringInt d, uint64_t key, int index) { //DictSearch
    struct elt *e;
    for(e = d->table[key % d->m]; e != 0; e = e->next) {
        if(e->key == key) {
            return e->index;
        }
    }
    return -1;
}

int DictFind(DictStringInt d, uint64_t key, unsigned char *value) { //When printing compressed characters
    struct elt *e;
    for(e = d->table[key % d->m]; e != 0; e = e->next) {
        if(e->key == key) {
            putchar(e->compressChar);
            return 1;
        }
    }

    unsigned char first = value[0]; //Problem with comparison
    //If statement for escape characters 
    //if((first > COMPRESSED_CHAR_MAX) || (first < COMPRESSED_CHAR_MIN)){ //check for escape
    // if((first <= 255) && (first >= 128)){
    //     putchar(COMPRESSED_CHAR_ESCAPE);
    //     putchar(first);
    // }    
    // else {
    //     putchar(first);
    // }
    // if ((first >= 0) && (first <= 255)){
    //     putchar(first);
    // }
    // else{
    //     putchar(COMPRESSED_CHAR_ESCAPE);
    //     putchar(first);
    // }
    if (first >= COMPRESSED_CHAR_MIN){
        putchar(COMPRESSED_CHAR_ESCAPE);
        putchar(first);
    }
    else{
        putchar(first);
    }

    return 0;
}

void arrayEltPrint(struct arrayElt **array, size_t length){ //For testing currently
    for (size_t i = 0; i < length; i++){
        printf("%s %d\n", array[i]->bigram, array[i]->count); 
    }
}

int compare(const void *a, const void *b){
    struct arrayElt **a2 = (struct arrayElt**) a;
    struct arrayElt **b2 = (struct arrayElt**) b;
    return (*b2)->count - (*a2)->count;
    //return ((struct arrayElt *)b)->count - ((struct arrayElt *)a)->count;
}


int main(int argc, char **argv) {

    //Create first dictionary to hash in bigrams and their index in arrayElts
    DictStringInt d = dictStringIntCreateInternal(INITIAL_SIZE);
    //Array of struct arrayElts, which contain bigram and frequency count
    struct arrayElt **bgArray = calloc(d->m, sizeof(struct arrayElt *));

    //Read in from stdin
    int a = getchar(); //Right types?
    int b; 
    

    //Create string for saving input
    size_t csize = 32;
    size_t cpos = 0;
    unsigned char *BigStr = malloc(32);

    BigStr[cpos]=a;
    cpos++;
    size_t arrayIndex = 0;
    size_t strLength = 1;

    while ((b = getchar()) != EOF) { //case when only once character is given, deal with later
        //Create string of input to use later
        BigStr[cpos] = b;
        cpos++;
        if(cpos == csize){
            csize *= SIZE_MULTIPLIER;
            BigStr = realloc(BigStr, csize);
        }

        //Create bigram
        unsigned char *bigram = malloc((2 * sizeof(int)));
        bigram[0] = a;
        bigram[1] = b;  

        //Search in hashtable. If not found, hash in and add to array. Else increase array index elt
        uint64_t tempHash;
        tempHash = FNV1a(bigram);
        int index = -1;
        index = indexFind(d, tempHash, index); //banana  ba an na an na  ba 1 an 2 na 2

 
        if (index == -1){ //If not found
            //Hash in bigram
            dictInsert(d, bigram, tempHash, arrayIndex);
            //Input into array
            struct arrayElt *e = malloc(sizeof(struct arrayElt));;
            e->count = 1;
            e->bigram = malloc(2*sizeof(char));
            e->bigram[0] = bigram[0];
            e->bigram[1] = bigram[1];
            bgArray[arrayIndex] = e;
            arrayIndex++;
        }
        else { //If found
            bgArray[index]->count++;
        }
        strLength++;
        free(bigram);
        a = b;
    }
//Sort that array

    qsort(bgArray, arrayIndex, sizeof(struct arrayElt *), compare); //d hashtable becomes useless now
    // printf("%zu\n", arrayIndex); how many bigrams are there in total
    // arrayEltPrint(bgArray, arrayIndex);
    
//Make dictionary d2 using the sorted array up to the 127 most common bigrams
    DictStringInt d2 = dictStringIntCreateInternal(127);

//Print out dictionary (254 values)
    int compress = COMPRESSED_CHAR_MIN;
    struct arrayElt *e2;
    //struct arrayElt *e2 = malloc(sizeof(*e2)); //What is right?
    int h = 127;
    if (arrayIndex < 127){
        h = arrayIndex;
    }
    for (size_t i = 0; i < h; i++){ 
        e2 = bgArray[i];
        putchar(e2->bigram[0]);
        putchar(e2->bigram[1]);
        dictInsert2(d2, e2->bigram, FNV1a(e2->bigram), compress);
        compress++;
    }
    //printf("%d", compress);

    while (h < 127){
        putchar('x');
        putchar('y');
        h++;
    }
        
//Print compressed values

    if(a != -1){ //Don't run if blank file
        for (size_t i = 0; i < cpos; i++){

            unsigned char *searchResult = malloc((2 * sizeof(char)));
            searchResult[0] = BigStr[i];
            
            unsigned int first = searchResult[0];

            if ((i+1) == cpos){ //When at end of string and last character
                //if((first > COMPRESSED_CHAR_MAX) || (first < COMPRESSED_CHAR_MIN) || (first==COMPRESSED_CHAR_ESCAPE)){ //check for escape
                // if((first <= 255) && (first >= 128)){
                //     putchar(COMPRESSED_CHAR_ESCAPE);
                //     putchar(first);
                // }
                // else {
                //     putchar(first);
                // }

                if (first >= COMPRESSED_CHAR_MIN){
                    putchar(COMPRESSED_CHAR_ESCAPE);
                    putchar(first);
                }
                else{
                    putchar(first);
                }
                
                free(searchResult);
                break;
            }
            searchResult[1] = BigStr[i+1];

            int f;
            if((f = DictFind(d2, FNV1a(searchResult), searchResult)) == 1){
                i++;
            }
            free(searchResult);
        }
    }

    free(BigStr); 
    ArrayDestory(bgArray, arrayIndex); 
    DictDestroy(d2); DictDestroy(d); 
    return 0;
}
// if ((first >= 0) && (first <= 255)){
//     if((first <= COMPRESSED_CHAR_MAX) && (first >= COMPRESSED_CHAR_MIN)){
//         putchar(COMPRESSED_CHAR_ESCAPE);
//         putchar(first);
//     }
//     else {
//         putchar(first);
//     }
// }
// else{
//     putchar(COMPRESSED_CHAR_ESCAPE);
//     putchar(first);
// }


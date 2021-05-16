#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <inttypes.h>
#include "decrypt.h"
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
    uint64_t key;  // char?, null-terminated character string
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

            //free(e->key);
            free(e->value);
            free(e);
        }
    }
    free(d->table);
    free(d);
}


//Hashing
void FNV1a(uint64_t *hash, char *s) {
    //uint64_t h = FNV_OFFSET_BASIS_64;
    while(*s) {
        *hash ^= *s++;
        *hash *= FNV_PRIME_64;
    }
    //return h;
}

uint64_t precomputeSalt(char *salt){
    uint64_t hash = FNV_OFFSET_BASIS_64;
    FNV1a(&hash, salt);
    return hash;
}
uint64_t passwordHash(uint64_t saltHash, char *password){
    FNV1a(&saltHash, password);
    return(saltHash);
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
void dictInsert(DictStringInt d, char *value, uint64_t key){
    struct elt *e = malloc(sizeof(struct elt));
    //uint64_t h = FNV1a(key);
    

    //strdup has built-in malloc, needs to be freed in destroy
    e->key = key; //make copy of uint hash key

    e->value = strdup(value); //make copy of char value
    unsigned long long h = key % d->m;

    e->next = d->table[h]; //SegFault
    //printf("he");

    d->table[h] = e;
    d->n++;
    


    /* grow table if there is not enough room */
    if(d->n >= (d->m * MAX_LOAD_FACTOR)) {
        dictGrow(d);
    }
    
}

char *DictSearch(DictStringInt d, uint64_t key) {
    struct elt *e;
    //for(e = d->table[FNV1a(key)]; e != 0; e = e->next) {
    for(e = d->table[key % d->m]; e != 0; e = e->next) {
        //if(!strcmp(e->key, key)) { //Not equal to zero
        if(e->key == key) {
            return e->value;
        }
    }
    return 0;
}

void dictStringIntPrint(DictStringInt d) {
    // walk through all table entries
    for(size_t i = 0; i < d->m; i++) {
        for(struct elt *e = d->table[i]; e != 0; e = e->next) {
            printf("%" SCNx64 " %s\n", e->key, e->value);
        }
    }
}

char *getFileLine(FILE *in){
    int c;
    size_t size = 32;
    size_t top = 0;
    char *buffer = calloc(size, sizeof(char));
    assert(buffer);
    for(;;){
        switch((c=getc(in))){
            case EOF:
                if(top==0){
                    free(buffer);
                    return 0;
                }
            case '\n':
                buffer[top] = '\0';
                return buffer;  
            default:
                buffer[top++] = c;
                if (top>=size){
                    size *= 2;
                    buffer= realloc(buffer, size*sizeof(char));
                    assert(buffer);
                }
        }
    }
}


int main(int argc, char **argv) {
    if (argc != 3){
        return 1;
    }
    //Read in password dictionary, concat with salt, hash, put in dictionary
    DictStringInt d = dictStringIntCreateInternal(INITIAL_SIZE);

    FILE *inFile = fopen(argv[2], "r");
    char *passIn = NULL;

    //make sure arg1 is non empty string
    uint64_t saltHash = precomputeSalt(argv[1]);
    
    while ((passIn = getFileLine(inFile)) != NULL) { //b=password
        uint64_t s = passwordHash(saltHash, passIn);
        //printf("%" SCNx64 "\n", s);
        dictInsert(d, passIn, s); 
        free(passIn);
    }

    
    //dictStringIntPrint(d);
    //printf("\n");


    
    //Read in encrpyted password until ":"
    //Then, compare hash with hash table
    int c;
    while ((c = getchar()) != EOF){
        putchar(c);
        if(c == ':') { //Getting hashed from encrypted file
            //char *password = getLine();
            uint64_t hash;
            scanf("%" SCNx64 "\n", &hash);
            //assert(hash);
            //printf("%" SCNx64 "\n", &hash);


            char *returnVal = DictSearch(d, hash);
            if (returnVal != 0) {
                printf("%s\n", returnVal);
            }
             else{
                printf("%" SCNx64 "\n", hash); 
             }
        }

    }
    fclose(inFile);

    DictDestroy(d);
    return 0;

}



//./encrypt salt <. turtles.password | ./decrypt salt passwords.dict


// char *b = NULL;
    // FILE *inFile = fopen(argv[2], "r");
    // size_t zero = 0;
    // size_t saltLength = strlen(argv[1]);
    

    // size_t lineLength = 0;
    // while ((lineLength = getline(&b, &zero, inFile)) != -1) { //b=password
    //     size_t length = saltLength + lineLength;
    //     char *s = connectSalt(argv[1], b, length);
    //     //uint64_t saltHash = FNV1a(s);
    //     dictInsert(d, b, s); //What do i send into as key
    //     free(s);
    // }

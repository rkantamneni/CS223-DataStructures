#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <inttypes.h>
#define INITIAL_SIZE (1024)
#define SIZE_MULTIPLIER (2)
#define MAX_LOAD_FACTOR (1)
#define FNV_PRIME_64 ((1ULL<<40)+(1<<8)+0xb3)
#define FNV_OFFSET_BASIS_64 (14695981039346656037ULL)

char *connectSalt(char *s1, char *s2 , size_t length) { //Online
    char *result = malloc(length + 1); // +1 for the null-terminator
    strcpy(result, s1);
    strcat(result, s2); //Look at this
    return result;
}

uint64_t FNV1a(char *s) {
    uint64_t h = FNV_OFFSET_BASIS_64;
    while(*s != '\0') {
        h ^= *s++;
        h *= FNV_PRIME_64;
    }
    return h;
}

int main(int argc, char **argv) {
    // char *d = "saltpassword";
    // printf("%" PRIx64 "\n", FNV1a(d));

    // uint64_t hash;
    // scanf("%" SCNx64 "\n", &hash);
    // printf("%" PRIx64 "\n", hash);
    // scanf("%" SCNx64 "\n", &hash);
    // printf("%" PRIx64 "\n", hash);
    // scanf("%" SCNx64 "\n", &hash);
    // printf("%" PRIx64 "\n", hash);

    // char *b = NULL;
    // FILE *inFile = fopen(argv[2], "r");
    // size_t zero = 0;
    // size_t lineLength = 0;
    // while ( (lineLength = getline(&b, &zero, inFile)) != -1) { 
    //     printf("%s", b);
    // }
    int a;
    while((a = getchar()) != EOF){
        printf("%x\n", a);
    }
    

}
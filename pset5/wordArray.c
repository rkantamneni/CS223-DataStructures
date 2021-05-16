#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "wordArray.h"

// interface for arrays keyed on fixed-length strings
// of lowercase letters

struct wordArray{
    int *array;
    unsigned long long keyLength;
};

// some constants for key elements
// #define MIN_KEY_LETTER ('a') //97
// #define MAX_KEY_LETTER ('z') //122
// #define NUM_KEY_LETTERS (MAX_KEY_LETTER - MIN_KEY_LETTER + 1) //26

// typedef struct wordArray{
//     int *array;
//     size_t keyLength;
// } *WordArray;



unsigned long long powerFunction (int x, int h) {
    int i; 
    unsigned long long number = 1;

    for (i = 0; i < h; ++i){
        number *= x;
    }    

    return number;
}

// create an array indexed by strings of length n
WordArray wordArrayCreate(unsigned int n){
    WordArray w = malloc(sizeof(struct wordArray)); 

    w->array = calloc(powerFunction(26, n), sizeof(int)); 
    w->keyLength = n;
    return w;
}

// free all space used by array
void wordArrayDestroy(WordArray w){
    free(w->array);
    free(w);
}

// get the key length n for an array
int wordArrayKeyLength(WordArray w){
    return w->keyLength;
}


// Returns a pointer to location in word array
// indexed by string key.
//
// If key is the wrong length, or contains
// characters that are not between MIN_KEY and MAX_KEY,
// returns 0.
int *wordArrayRef(WordArray w, const char *key){
    if (!(wordArrayKeyLength(w) == strlen(key))) {
        return 0;
    }

    //Loop for each character in key, make sure lowercase
    for (int i = 0; key[i] != '\0'; i++) { 
        if (!islower(key[i])) {
            return 0;
        }      
    }
    
    unsigned long long index = 0;
    //abc
    for (unsigned long long i = 0; i < w->keyLength; i++){
        index = index + (key[i]-'a') * (powerFunction(26, i));
    }

    return &(w->array[index]);       
    
}

// returns malloc'd minimum key for w
// as a null-terminated string
// wordArrayMinKey that returns a freshly-malloc’d string of a characters of the appropriate length for a given WordArray

char *wordArrayMinKey(WordArray w){ //add null terminator at end
    char *minKey = malloc((sizeof(char) * w->keyLength) + 1);

    for (unsigned long long i = 0; i < w->keyLength; i++){
        minKey[i] = MIN_KEY_LETTER;
    }

    minKey[w->keyLength] = '\0';
    
    return minKey;
}

// Increments a string in place, returning 1 on overflow
// and 0 otherwise
// 
// Examples:
//
// incKey("aaa") -> "aab"
// incKey("abc") -> "abd"
// incKey("abz") -> "aca"
// incKey("zzz") -> "aaa", returns 1
//zaz -> zba
//incKey("") -> overflow, returns 1
int wordArrayIncKey(char *s){
    int length = strlen(s);

    if (length == 0){
        return 1;
    }
    //int truth = 0;
    for(int i = length-1; i >= 0; i--){
        // if (truth == 1){
        //     printf("hello");
        //     s[i]++;
        //     if (s[i] > 'z'){
        //         s[i] = 'a';
        //         return 1;
        //     }
        //     truth = 0;
        // }
        if (s[i] == 'z'){
            if (i==0){
                s[i] = 'a';
                return 1;
            }
            else{
                s[i] = 'a';          
            }          
        }
        else {
            s[i]++;
            return 0;
        }  
    }

    return 0; //Don't really need

}


// print the contents of an array,
// with one key and value on each line, as in
//
// aa 0
// ab 0
// ac 0
// ...
// 

void wordArrayPrint(WordArray w){
    char *key = wordArrayMinKey(w); //Need to free wordArrayMinKey
    do {
        printf("%s ", key);
        printf("%d\n", *wordArrayRef(w, key)); //New line if at end?
        
    } while(wordArrayIncKey(key) == 0);

    free(key);    
}


// int main(int argc, char **argv) {
//     WordArray w = wordArrayCreate(3);
//     //*wordArrayRef(w, "aaa") = 12;
//     // printf("%d\n", *wordArrayRef(w, "zz"));
//     wordArrayPrint(w);
// }

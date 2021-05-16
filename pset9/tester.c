#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#define LEFT_BRACKET ('{')
#define RIGHT_BRACKET ('}')
#define SIZE_MULTIPLIER (2)

char *finalString = malloc(inputSize);
    if (i==inputSize){ 
        duringFlipString = flip(duringFlipString, duringFlipStringLength);
        size_t p = 0;
        for (size_t w = beforeFlipStringLength; w < (duringFlipStringLength-numOfBrackets); w++){
            beforeFlipString[w] = duringFlipString[p];
            p++;
        } 
        return beforeFlipString;    
    }
    else{
        unsigned char *afterFlipString = malloc(inputSize);
        int g = 0;
        while (i < inputSize){
            afterFlipString[g] = InputString[i];
            i++;
        }
        int afterFlipStringLength = i-beforeFlipStringLength-duringFlipStringLength;
        afterFlipString = flip(afterFlipString, afterFlipStringLength);
        return beforeFlipString;
    }

int main(int argc, char **argv) {
    size_t reallocSize = 32;
    size_t characterPosition = 0;
    size_t inputSize = 0;
    unsigned char *InputString = malloc(32);
    size_t numBrackets = 0;
    int a;
    while ((a = getchar()) != EOF) {
        if ((a == LEFT_BRACKET) || (a == RIGHT_BRACKET)){
            numBrackets++;
        }
        InputString[characterPosition] = a;
        characterPosition++;
        if(characterPosition == reallocSize){
            reallocSize *= SIZE_MULTIPLIER;
            InputString = realloc(InputString, reallocSize);
        } 
        inputSize++;
    }


// while(close != 1){
//             if (InputString[i] == LEFT_BRACKET){
//                 close++;
//                 numOfBrackets++;
//             }
//             if (InputString[i] == RIGHT_BRACKET){
//                 close--;
//                 numOfBrackets++;
//             }
//             i++;
//         }


// if (i==inputSize){ 
//         finalString = strdup(duringFlipString);
//         duringFlipString = flip(finalString, duringFlipStringLength2); //13
//         size_t p = 0;
//         for (size_t w = beforeFlipStringLength; w < (duringFlipStringLength-numOfBrackets); w++){
//             beforeFlipString[w] = duringFlipString[p];
//             p++;
//         } 
//         return finalString; 
//     }

// for (size_t w = beforeFlipStringLength; w < (inputSize-numOfBrackets+4); w++){
//             if (w < (duringFlipStringLength+beforeFlipStringLength-2)) { 
//                 beforeFlipString[w] = duringFlipString[p];
//                 p++;
//             }
//             else{
//                 beforeFlipString[w] = afterFlipString[u];
//                 // putchar(afterFlipString[u]);
//                 // putchar('\n');
//                 u++;
//             }
//         } 



//Make copy for freeing purposes
            // for (size_t i = 0; i < (inputSize-numOfBrackets); i++){
            //     finalString[i] = beforeFlipString[i];
            // }
             

        //} 
        //Free memory
        //free(beforeFlipString);
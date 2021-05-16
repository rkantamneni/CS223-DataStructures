//use control d for eof in terminal
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#define LEFT_BRACKET ('{')
#define RIGHT_BRACKET ('}')
#define SIZE_MULTIPLIER (2)
// abc{uoy {are} woH} {?yadot}
// abc How are you today?

char *flip(char *InputString, size_t inputSize){ //comes in as unflipped

//Part 1, Characters before brackets, base case
    int i = 0;
    char *beforeFlipString = malloc(inputSize);
    while(InputString[i] != LEFT_BRACKET){
        beforeFlipString[i] = InputString[i];
        i++;
        if (i==inputSize){
            return beforeFlipString;
        }
    }
    int beforeFlipStringLength = i;

//Part 2, Characters in brackets
    char *duringFlipString = malloc(inputSize);
    int numOfBrackets = 0;
    if (InputString[i] == LEFT_BRACKET){
        int leftBrackPos = i; //Position in inputString
        
        //Find right brack position
        int numOfLeftBrackets = 0;
        int numOfRightBrackets = 0;
        while((numOfLeftBrackets != numOfRightBrackets) || (numOfLeftBrackets == 0) || (numOfRightBrackets == 0)){
            if (InputString[i] == LEFT_BRACKET){
                numOfLeftBrackets++;
                numOfBrackets++;
            }
            if (InputString[i] == RIGHT_BRACKET){
                numOfRightBrackets++;
                numOfBrackets++;
            }
            i++;
        }

        int rightBrackPos = i-1;
        //Reverse it based on bracket position
        int j = 0;
        if (rightBrackPos != leftBrackPos){
            for (size_t k = rightBrackPos-1; k > leftBrackPos; k--){
                if (InputString[k] == LEFT_BRACKET){
                    duringFlipString[j] = RIGHT_BRACKET;
                }
                else if (InputString[k] == RIGHT_BRACKET){
                    duringFlipString[j] = LEFT_BRACKET;
                }
                else {
                    duringFlipString[j] = InputString[k];
                }
                j++;
            }
        }
    }
    int duringFlipStringEndPos = i;
    int duringFlipStringLength2 = i-beforeFlipStringLength-2; 

    // printf("%zu\n\n", inputSize);
    // fflush(stdout);

//Part 3, Characters after brackets
    if (i==inputSize){ 
        char *duringFlip2 = NULL;
        if (duringFlipStringLength2 != 0){
            duringFlip2 = flip(duringFlipString, duringFlipStringLength2);
        }
        size_t p = 0;
        for (size_t w = beforeFlipStringLength; w < (inputSize-numOfBrackets); w++){
            beforeFlipString[w] = duringFlip2[p];
            p++;
        }
        if (duringFlip2 != NULL){
            free(duringFlip2);
        }
        free(duringFlipString);
        return beforeFlipString; 
    }

    else{
        char *afterFlipString = malloc(inputSize);
        int g = 0;
        int numOfBrackets2 = 0;
        while (i < inputSize){
            afterFlipString[g] = InputString[i];
            if ((InputString[i] == LEFT_BRACKET) || (InputString[i] == RIGHT_BRACKET)){
                numOfBrackets2++;
            }
            i++;
            g++;
        }
        int afterFlipStringLength = g;
        char *afterFlip2 = flip(afterFlipString, afterFlipStringLength);
        char *duringFlip2 = NULL;
        if (duringFlipStringLength2 != 0){
             duringFlip2 = flip(duringFlipString, duringFlipStringLength2); //13
        }
        size_t p = 0;
        size_t u = 0;
        for (size_t w = beforeFlipStringLength; w < (inputSize-(numOfBrackets+numOfBrackets2)); w++){
            if (w < (duringFlipStringEndPos-numOfBrackets)) { 
                beforeFlipString[w] = duringFlip2[p];
                p++;
            }
            else{
                beforeFlipString[w] = afterFlip2[u];
                u++;
            }
        } 
        //Free memory
        if (duringFlip2 != NULL){
            free(duringFlip2);
        }
        free(duringFlipString);
        free(afterFlipString);
        free(afterFlip2);

        return beforeFlipString; 
    }          
}


int main(int argc, char **argv) {
    size_t reallocSize = 32;
    size_t characterPosition = 0;
    size_t inputSize = 0;
    char *InputString = malloc(32);
    size_t numBrackets = 0;
    int empty = 1;
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
        empty = 0;
    }
    if (empty != 1){
        char *printOutput = flip(InputString, inputSize);
        for (size_t i = 0; i < (inputSize-numBrackets); i++){
            putchar(printOutput[i]);
        }
        free(printOutput); //AKA freeing beforeOutput
    }

    //Free
    free(InputString);
}

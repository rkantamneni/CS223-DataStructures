#include "printFixed.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

//**Assignment Plan/Notes**
//unsigned long long int as input

//bigger or smaller than the precision point
//getting number as a long long

void printFixed(long long number, char separator, 
char decimalPoint, unsigned int precision){
    
//Calculate no. of digits
    int inputLength = 0;
    long long nCount = number;
    long long arrayBigNum = number;
    int isNegative = 0;
    
    
    if (number < 0){ //Is the number negative?
        printf("-");
        isNegative = 1;
    }

    while (nCount != 0){
        nCount /= 10;
        inputLength++;
    }
    
//**Creates Array and give size**
    //unsigned int = range(0 to 65,535 or 0 to 4,294,967,295)
    //You may assume that precision is small enough 
    //that 10^precision fits in a long long

    int arraySize = inputLength;

    char numArray[arraySize]; //change to long long
    int count2 = inputLength; //Change this to array size

    if (count2 != 0) {
        count2 = 0;
        while (arrayBigNum != 0) {
            if (isNegative == 1){
                numArray[count2] = (arrayBigNum % 10) * -1;
            }
            else {
                numArray[count2] = (arrayBigNum % 10); //add 48 if we use char for array
            }
            arrayBigNum /= 10;
            count2++;
        }
    }

    // for (int i = 0; i < arraySize; i++){
    //     printf("%d", numArray[i]);
    // }


//Zero Stuff/Less Than One Output

    int numOfZeros;

    if (inputLength <= precision) {
        numOfZeros = (precision-inputLength);

        printf("0");
        printf("%c", decimalPoint);
        for (int x = 0; x < numOfZeros; x++){
            printf("0");
        }
        for (int i = arraySize; i > 0; i--){
            printf("%c", numArray[i-1]);
        }
    }

//Greater Than One Output 

    else { 
        long long commaPlace = arraySize - precision;  //  6 - 4 = After two number

        for (int i = arraySize; i > 0; i--){
            printf("%d", numArray[i-1]); 

            if (i == (precision+1)){
                printf("%c", decimalPoint);
            }
            else {
                if (((commaPlace%3) == 1) && (commaPlace > 3)){
                    printf("%c", separator);
                }
            }
            commaPlace--;
        }
    }  
}










#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>


int main(int argc, char **argv) {
    size_t mycount = 0;        /* number of numbers read */
    size_t size = 1;           /* size of block allocated so far */

    //Size and my count size_t
    char *a;                 /* block */


    a = malloc(sizeof(char) * size);     /* allocating zero bytes is tricky */
    //if (a == 0) return 0;

    int g;
    while((g = getchar()) != EOF) {
        /* is there room? */
        while(mycount >= size) {
            /* double the size to avoid calling realloc for every number read */
            size *= 2;
            a = realloc(a, sizeof(char) * size);
            //if(a == 0) return 0;
        }

        /* put the new number in */
        a[mycount] = g;
        mycount++;
    }

    /* now trim off any excess space */
   // a = realloc(a, sizeof(int) * mycount);

    char *resultArray = malloc(sizeof(char) * mycount);


    //Make array with all spaces and special symbols
    for (int i = 0; i < mycount; i++){
        if (!isalpha(a[i])){
            resultArray[i] = a[i];
        }
        else if (isupper(a[i])) {
            resultArray[i] = 'A';
        }
        else {
            resultArray[i] = 'a';
        }
    } 


    size_t j = mycount-1;

    //Add in letters to resultArray
    for (int i = 0; i < mycount; i++){
        if (isalpha(a[i])) {  
            //My name is Ozymandias, King of Kings;

            while (resultArray[j]!='A' && resultArray[j]!='a'){
                j--;
            }

            if (resultArray[j]=='A'){
                resultArray[j] = toupper(a[i]);
            }
            else {
                resultArray[j] = tolower(a[i]);  
            } 

            j--;
        }    
    }


    //Print out Reverse
    for (int i = 0; i < mycount; i++){
        printf("%c", resultArray[i]);  
    }  

    free(a);
    free(resultArray);
}



// Ri apse dd Naythgimey, Skro wy Mnoko;
// Olsg ni kf Ognik, sa Idnamy, zos iemanym!
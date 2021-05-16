#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "buffer.h"

struct buffer{
    size_t numCursors;
    size_t characterNum;
    struct elt **cursors; //array of struct elemants
    struct elt *top; 
};

struct elt{
    struct elt *prior; //next element in list, or 0 at end of list
    struct elt *next; //next element in list, or 0 at end of list
    int value; //stored value
};


//What about 0 cursors?
//size t error from it always being greater than zero

// Make a buffer with n cursors.
Buffer bufferCreate(size_t n){

    Buffer b = malloc(sizeof(struct buffer));

    struct elt *nullElt = malloc(sizeof(struct elt));
    b->top = nullElt;
    
    nullElt->next = NULL;
    nullElt->prior = NULL;
    nullElt->value = '\0';

    if (n!=0) {
        b->cursors = calloc(n, sizeof(struct elt*)); //pointer to struct element
        for (size_t i = 0; i < n; i++){
            b->cursors[i] = b->top; //assign each cursor an address of NULL
        }
    }

    b->characterNum = 1;
    b->numCursors = n;
    return b;
}

// Free all space used by a buffer.
void bufferDestroy(Buffer b){
    //free linked list
    struct elt *temp = b->top;  //temp variable pointing to top
    while (temp != NULL) {
        struct elt *temp2 = temp;
        temp = temp->next;
        free(temp2);
    }

    if (b->numCursors != 0){
        free(b->cursors); //free cursors array
    }
    
    free(b);
}

// Insert a new non-null character before the i-th cursor position.
// All cursors continue to point to the same character
// they pointed to before.
// If i is out of range or c is '\0', has no effect.
void bufferInsert(Buffer b, size_t i, char c){
    if (c!='\0' && i < b->numCursors){
        b->characterNum+=1;

        
        //New element assignment
        struct elt *newElt = malloc(sizeof(struct elt));
        newElt->value = c;
        newElt->next = b->cursors[i];
        newElt->prior = b->cursors[i]->prior;
             
        //Prior element assignment
        // 1 2 3 4 5
        if (b->cursors[i] == b->top){
            b->top = newElt;
        }
        else{
            b->cursors[i]->prior->next = newElt;
        }
        
        b->cursors[i]->prior = newElt;
       
    }

}

// Advance cursor i one position.
// If i is out of range, or the i-th cursor
// already points to the final '\0',
// has no effect.

// struct elt *prior; //next element in list, or 0 at end of list
//     struct elt *next; //next element in list, or 0 at end of list
//     int value; //stored value
void bufferCursorForward(Buffer b, size_t i){
    
    if (i < b->numCursors && b->cursors[i]->value!='\0'){
        b->cursors[i] = b->cursors[i]->next;
    }
}

// Move cursor i back one position.
// If i is out of range, or the i-th cursor
// already points to the first character,
// has no effect.
void bufferCursorBack(Buffer b, size_t i){
    if (i<b->numCursors && b->cursors[i]!=b->top){
        b->cursors[i] = b->cursors[i]->prior;
    }
}

// Return the number of cursors in a buffer
size_t bufferCursors(Buffer b){
    return b->numCursors;
}

// Return the number of characters in a buffer,
// not including the final null.
size_t bufferSize(Buffer b){
    return b->characterNum-1;
}

// Return the characters in a buffer
// as a null-terminated sequence of chars.
// Return value is malloc'd and should be freed by caller.
char *bufferContents(Buffer b){
    char *string = malloc(sizeof(char) * b->characterNum);

    //Need to add null terminating string?
    struct elt *temp3 = b->top;

    //for (size_t i = 0; i < b->characterNum; i++){
    size_t i = 0;
    while (temp3 != NULL) {
        //start at top, and go on to "next"
        string[i] = temp3->value;
        temp3 = temp3->next;
        i++;
    }
   
    return string;
}




// int main(int argc, char **argv) {
//     Buffer b = bufferCreate(3);
//    
// }
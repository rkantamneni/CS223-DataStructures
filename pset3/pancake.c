#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "pancake.h"


void runPancakeProgram(unsigned int *memory){

    unsigned int programcounter = 0;
    // Flip  0  2  6

    while ((memory[programcounter] == 0) || (memory[programcounter] == 1) || (memory[programcounter] == 2) || (memory[programcounter] == 3)) {

        if (memory[programcounter]==0) { //Flip
            if (memory[programcounter+1]<memory[programcounter+2]) { 
                //FLIP	0	x y	Flip memory[x] through memory[y-1]

                int startFlip = memory[programcounter+1];
                int endFlip = memory[programcounter+2]-1;

                while (startFlip < endFlip) {
                    int dummy_startFlip = memory[startFlip];
                    int dummy_endFlip = memory[endFlip];
                    memory[startFlip] = dummy_endFlip;
                    memory[endFlip] = dummy_startFlip;
                    startFlip+=1;
                    endFlip-=1;
                }               
                // for (int i = 0; i < 10; i++) {
                //         printf("%d ", memory[i]);
                // } 
            }
            programcounter+=3;
        }
        
        else if (memory[programcounter]==1) { //Print
            unsigned int startPos = memory[programcounter+1];
            unsigned int endPos = memory[programcounter+2];

            for (unsigned int i = startPos; i < endPos; i++) {
                if (i+1==endPos){
                    printf("%u", memory[i]);
                }
                else{
                    printf("%u ", memory[i]);
                }     
            }
            printf("\n");
            programcounter+=3;
            
        }

        else if (memory[programcounter]==2){ //Jump
            programcounter = memory[programcounter+1];     
        }

        else if (memory[programcounter]==3) { //Skip Instruction
            if (memory[memory[programcounter+1]] > memory[memory[programcounter+2]]) {
                if ((memory[programcounter+3] == 0) || (memory[programcounter+3] == 1) || (memory[programcounter+3] == 3)){
                    programcounter+=6;
                }
                else if (memory[programcounter+3] == 2)
                {
                    programcounter+=5;
                }
                else {
                    programcounter+=4;
                }
            }

            else {
                programcounter+=3;
            }


        }  
        
    }
 
}





// #define INITIAL_SIZE (256)  // initial buffer size
// #define SIZE_MULTIPLIER (2) // how much to expand by
// static unsigned int *
// readInts(FILE *f, size_t *n)
// {
//     size_t size = INITIAL_SIZE;
//     size_t top = 0;
//     unsigned int *a = malloc(sizeof(unsigned int) * size);
//     unsigned int value;
//     assert(a);   // Linux malloc never fails, but let's try to be safe
//     while(fscanf(f, "%u", &value) == 1) {
//         if(top >= size) {
//             size *= SIZE_MULTIPLIER;
//             a = realloc(a, sizeof(unsigned int) * size);
//             assert(a);
//         }
//         a[top++] = value;
//     }
//     fclose(f);
//     if(n) {
//         *n = top;
//     }
//     return a;
// }
// #define HASH_MULTIPLIER (11400714819323198485u)
// #define HASH_SHIFT (32)
// // return a hash function of array a
// static unsigned long long
// hash(const unsigned int *a, size_t n)
// {
//     unsigned long long h = 0;
//     for(size_t i = 0; i < n; i++) {
//         h = ((h >> HASH_SHIFT) ^ h) * HASH_MULTIPLIER + a[i];
//     }
//     return h;
// }
// int
// main(int argc, char **argv)
// {
//     if(argc != 1) {
//         fprintf(stderr, "Usage: %s < memory-file\n", argv[0]);
//         return 1;
//     }
//     size_t n;
//     unsigned int *memory = readInts(stdin, &n);
//     runPancakeProgram(memory);
//     printf("%llx\n", hash(memory, n));
//     free(memory);
//     return 0;
// }



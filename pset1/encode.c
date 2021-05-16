#include <stdio.h>
#include <ctype.h>


int peekchar(void) { //code from CS223 web page
    int b = getchar();
    if (b != EOF) {
        ungetc(b, stdin);     
    }
    return b;
}

int main(int argc, char **argv) {
    int count = 0;
    int a;
    while((a = getchar()) != EOF) {

        if ((a == (peekchar())) && (count != 9)) {
            count++; 
        }
        else {
            if ((isdigit(a)) || (count > 1)){
                putchar(count+48);
                putchar(a);
            }
            else if (count == 1){
                putchar(a);
                putchar(a);
            }
            else {
                putchar(a);
            }
            count = 0;  
        }
    }
}

//if new line considered encoding

#include <stdio.h>
#include <ctype.h>


int peekchar(void) { //code from CS223 web page
    int c = getchar();
    if (c != EOF) {
        ungetc(c, stdin);     
    }
    return c;
}

int main(int argc, char **argv) {
    int a, b;           
    while((a = getchar()) != EOF) {

        if (isdigit(a) && (peekchar() == EOF)) {
            break;     
        }

        else if (isdigit(a)) {
            char count = a-48;
            if ((b = getchar()) != EOF) {
                for (int i = 0; i <= count; i++) {
                    putchar(b);
                }
            }
        }
        else {
            putchar(a);
        }
    }
}

//case when potato6

//single digits
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "inserter.h"

static void
listDestroy(struct element *head)
{
    while(head) {
        struct element *next = head->next;
        free(head->string);
        free(head);
        head = next;
    }
}

// With optional argument n, builds n lists,
// with input format <list-number> <item>.
//
// Without optional n, builds one list out
// of words on input with no list numbers.
int
main(int argc, char **argv)
{

    if(argc > 2) {
        fprintf(stderr, "Usage: %s [n]\n", argv[0]);
        return 1;
    }

    // number of inserters
    int n;
    if(argc > 1) {
        n = atoi(argv[1]);
    } else {
        n = 1;
    }

    struct element *heads[n];
    Inserter ins[n];

    for(int i = 0; i < n; i++) {
        heads[i] = 0;
        ins[i] = inserterCreate(&heads[i]);
    }

    int choice = 0;
    char *s;

    if(n > 1) {
        while(scanf("%d%ms", &choice, &s) == 2) {
            inserterAdd(ins[choice], s);
            free(s);
        }
    } else {
        while(scanf("%ms", &s) == 1) {
            inserterAdd(ins[choice], s);
            free(s);
        }
    }
    inserterAdd(ins[0], "b");
    inserterAdd(ins[0], "a");
    inserterAdd(ins[0], "c");
    inserterAdd(ins[0], "x");
    inserterAdd(ins[0], "j");
    inserterAdd(ins[0], "l");
    inserterAdd(ins[0], "p");
    inserterAdd(ins[0], "e");
    inserterAdd(ins[0], "s");
    inserterAdd(ins[0], "d");
    

    for(int i = 0; i < n; i++) {
        printf("=== %d ===\n", i);
        for(struct element *e = heads[i]; e; e = e->next) {
            puts(e->string);
        }

        inserterDestroy(ins[i]);
        listDestroy(heads[i]);
    }

    return 0;
}

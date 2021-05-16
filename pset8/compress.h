// Constants for the compressed file format.
//
// Each file starts with a table of
// COMPRESSED_CHARS * COMPRESSED_EXPANSION bytes,
// representing the expanded values of
// COMPRESSED_CHAR_MIN through COMPRESSED_CHAR_MAX.
//
// The rest of the file is a mix of ordinary characters
// (which are passed through unmodified),
// characters in the range COMPRESSED_CHAR_MIN
// through COMPRESSED_CHAR_MAX
// (which are replaced by their two-character expansions),
// and escape sequences consisting of
// COMPRESSED_CHAR_ESCAPE followed by an arbitrary
// character
// (which are replaced by the second character alone).
//
// An incomplete escape sequence at end of file is ignored.

#include <inttypes.h>
typedef struct dictStringInt *DictStringInt;

/* create a new empty dictionary */
DictStringInt DictCreate(void);

/* destroy a dictionary */
//void DictDestroy(DictStringInt);

/* insert a new key-value pair into an existing dictionary */
//void dictInsert(DictStringInt, char *value, uint64_t key);

/* return the most recently inserted value associated with a key */
/* or 0 if no matching key is present */
//void DictSearch(DictStringInt, uint64_t key, char *value);
// unsigned char *DictSearch(DictStringInt d, uint64_t key) {
//     struct elt *e;
//     for(e = d->table[key % d->m]; e != 0; e = e->next) {
//         if(e->key == key) {
//             return e->value;
//         }
//     }
//     return 0;
// }




#define MAX_LOAD_FACTOR (1)
#define COMPRESSED_CHAR_MIN (0x80)
#define COMPRESSED_CHAR_MAX (0xfe)
#define COMPRESSED_CHAR_ESCAPE (0xff)

// Each compressed char represents this many
// uncompressed chars.
#define COMPRESSED_EXPANSION (2)

// Number of compressed chars.
#define COMPRESSED_CHARS (COMPRESSED_CHAR_MAX - COMPRESSED_CHAR_MIN + 1)

// void dictStringIntPrint(DictStringInt d) {
//     for(size_t i = 0; i < d->m; i++) {
//         for(struct elt *e = d->table[i]; e != 0; e = e->next) {
//             //printf("%#x %s %zu\n", e->compress, e->value, e->count);
//             printf("%s", e->value);
//         }
//     }
// }

//e->value = strdup(value); //make copy of char value, need to free

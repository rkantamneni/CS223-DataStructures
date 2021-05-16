#include <inttypes.h>
typedef struct dictStringInt *DictStringInt;

/* create a new empty dictionary */
DictStringInt DictCreate(void);

/* destroy a dictionary */
void DictDestroy(DictStringInt);

/* insert a new key-value pair into an existing dictionary */
void dictInsert(DictStringInt, char *value, uint64_t key);

/* return the most recently inserted value associated with a key */
/* or 0 if no matching key is present */
char *DictSearch(DictStringInt, uint64_t key);

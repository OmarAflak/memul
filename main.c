#include <stdio.h>
#include <stdlib.h>
#include "memul.h"

int main() {
    memul_t* memory = memul_create();
    int start = memul_malloc(memory, 5);
    printf("start = %d\n", start);
    word_t* ptr = memul_get(memory, start, 0);
    *ptr = 21;
    printf("val = %d\n", *memul_get(memory, start, 0));
    memul_free(memory, start, 5);
    memul_destroy(memory);
}
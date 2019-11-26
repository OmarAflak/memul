#ifndef MEMUL_H
#define MEMUL_H

#include <stdio.h>
#include <stdlib.h>

typedef int16_t word_t;
#define FRAME_SIZE sizeof(word_t)
#define PAGE_LENGTH 32
#define PHY_MEMORY_LENGTH 1024
#define LOG_MEMORY_LENGTH 2048

typedef struct memul {
    word_t memory[FRAME_SIZE * PHY_MEMORY_LENGTH];
    word_t pages[FRAME_SIZE * LOG_MEMORY_LENGTH / PAGE_LENGTH];
    char lms[sizeof(char) * LOG_MEMORY_LENGTH / PAGE_LENGTH];
    char pms[sizeof(char) * PHY_MEMORY_LENGTH / PAGE_LENGTH];
} memul_t;

memul_t* memul_create();
int memul_malloc(memul_t* m, int sizeInWords);
word_t* memul_get(memul_t* m, int start, int offset);
void memul_free(memul_t* m, int start, int size);
void memul_destroy(memul_t* m);

#endif
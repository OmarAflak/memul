#include "memul.h"

memul_t* memul_create() {
    memul_t* m = malloc(sizeof(memul_t));

    // init memory
    for (int i=0 ; i<FRAME_SIZE * PHY_MEMORY_LENGTH ; ++i) {
        m->memory[i] = 0;
    }

    // init pages
    for (int i=0 ; i<LOG_MEMORY_LENGTH / PAGE_LENGTH ; ++i) {
        m->pages[i] = -1;
    }

    // init logical memory state
    for (int i=0 ; i<LOG_MEMORY_LENGTH / PAGE_LENGTH ; ++i) {
        m->lms[i] = 0;
    }

    // init physical memory state
    for (int i=0 ; i<PHY_MEMORY_LENGTH / PAGE_LENGTH ; ++i) {
        m->pms[i] = 0;
    }

    return m;
}

int memul_malloc(memul_t* m, int sizeInWords) {
    if (sizeInWords == 0) {
        return -1;
    }

    // number of pages to allocate
    int q = sizeInWords / PAGE_LENGTH;
    if (sizeInWords % PAGE_LENGTH > 0) {
        q += 1;
    }

    // find space in physical memory
    int counter = 0;
    int pmi[q];
    for (int i=0 ; i<PHY_MEMORY_LENGTH / PAGE_LENGTH ; ++i) {
        if (m->pms[i] == 0) {
            pmi[counter++] = i;
            if (counter == q) {
                break;
            }
        }
    }

    // if not enough memory, exit
    if (counter < q) {
        return -1;
    }

    // find contiguous space in logical memory
    counter = 0;
    int start = -1;
    for (int i=0 ; i<LOG_MEMORY_LENGTH / PAGE_LENGTH ; ++i) {
        if (m->lms[i] == 0) {
            if (start == -1){
                start = i;
            }
            counter++;

            if (counter == q) {
                break;
            }
        } else {
            start = -1;
            counter = 0;
        }
    }

    // if no contiguous space found (unlikely), exit
    if (start == -1) {
        return -1;
    }

    // all good, start allocation : update pages and memory state
    for (int i=start ; i<start + q ; ++i) {
        m->pages[start] = pmi[i - start];
        m->pms[pmi[i - start]] = 1;
        m->lms[i] = 1;
    }

    // return start index of memory in pages
    return start;
}

word_t* memul_get(memul_t* m, int start, int offset) {
    int q = offset / PAGE_LENGTH;
    int r = offset % PAGE_LENGTH;
    int pi = m->pages[start + q] + r;
    return &m->memory[pi];
}

void memul_free(memul_t* m, int start, int size) {
    // number of pages to free
    int q = size / PAGE_LENGTH;
    if(size % PAGE_LENGTH > 0) {
        q += 1;
    }

    for (int i=start ; i<start + q ; ++i) {
        m->pms[m->pages[i]] = 0;
        m->lms[i] = 0;
    }
}

void memul_destroy(memul_t* m) {
    free(m);
}
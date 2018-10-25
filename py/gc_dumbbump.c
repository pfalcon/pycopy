/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Paul Sokolovsky
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <string.h>

#include "py/gc.h"
#include "py/runtime.h"

#define GC_ENTER()
#define GC_EXIT()

static char *heap_start, *heap_end, *heap_ptr;

void gc_init(void *start, void *end) {
    heap_ptr = heap_start = start;
    heap_end = end;
}

void *gc_alloc(size_t n_bytes, bool has_finaliser) {
    if (!n_bytes) {
        return NULL;
    }

    n_bytes = (n_bytes + sizeof(void*) - 1) & ~(sizeof(void*) - 1);

    if (heap_end - heap_ptr < n_bytes) {
        return NULL;
    }

    void *p = heap_ptr;
    heap_ptr += n_bytes;
    return p;
}

void *gc_realloc(void *ptr, size_t n_bytes, bool allow_move) {
    if (!allow_move) {
        return NULL;
    }

    void *new = gc_alloc(n_bytes, false);
    // "Conservatively" just copy as much as we were requested.
    if (ptr != NULL && new != NULL) {
        memcpy(new, ptr, n_bytes);
    }

    return new;
}

void gc_free(void *ptr) {
    if (ptr == NULL) {
        return;
    }
}

void gc_collect_start(void) {
}

void gc_collect_end(void) {
}

void gc_collect_root(void **ptrs, size_t len) {
}

size_t gc_nbytes(const void *ptr) {
    return 0;
}

void gc_info(gc_info_t *info) {
}

void gc_dump_info(void) {
}

void gc_dump_alloc_table(void) {
}

void gc_lock(void) {
    GC_ENTER();
    MP_STATE_MEM(gc_lock_depth)++;
    GC_EXIT();
}

void gc_unlock(void) {
    GC_ENTER();
    MP_STATE_MEM(gc_lock_depth)--;
    GC_EXIT();
}

bool gc_is_locked(void) {
    return MP_STATE_MEM(gc_lock_depth) != 0;
}

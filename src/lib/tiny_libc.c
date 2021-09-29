/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <tiny-libc/tiny_libc.h>

// #define TC_MEM_LEAK_FINDER

#if defined TC_MEM_LEAK_FINDER
typedef struct tc_allocation {
    const char* source_file;
    int line_number;
    void* pointer;
    int is_allocated;
    size_t size;
} tc_allocation;

typedef struct tc_allocations {
    tc_allocation* allocations;
    size_t max_count;
} tc_allocations;

tc_allocations g_allocations;
#endif

void tc_alloc_init()
{
#if defined TC_MEM_LEAK_FINDER
    tc_allocations* self = &g_allocations;
    self->max_count = 30000;
    self->allocations = calloc(self->max_count, sizeof(tc_allocation));
#endif
}
#if defined TC_MEM_LEAK_FINDER

static void debug_ex(tc_allocations* self)
{
    CLOG_INFO("Memory Leaks ----------------");
    for (size_t i = 0; i < self->max_count; ++i) {
        const tc_allocation* a = &self->allocations[i];
        if (a->is_allocated) {
            CLOG_INFO("%s:%d size:%zu", a->source_file, a->line_number, a->size);
        }
    }
    CLOG_INFO("------------------ Memory Leaks");
}

static void tc_alloc_debug()
{
    debug_ex(&g_allocations);
}

static tc_allocation* find_free_allocation(tc_allocations* self)
{
    for (size_t i = 0; i < self->max_count; ++i) {
        tc_allocation* a = &self->allocations[i];
        if (!a->is_allocated) {
            return a;
        }
    }

    return 0;
}

static tc_allocation* find_allocation_from_pointer(tc_allocations* self, void* p)
{
    for (size_t i = 0; i < self->max_count; ++i) {
        tc_allocation* a = &self->allocations[i];
        if (a->pointer == p) {
            return a;
        }
    }

    return 0;
}
#endif

void tc_alloc_reset()
{
#if defined TC_MEM_LEAK_FINDER
    tc_allocations* self = &g_allocations;
    debug_ex(self);
#endif
}

void* tc_malloc_debug(size_t size, const char* source_file, int line_number)
{
    void* p = malloc(size);
#if defined TC_MEM_LEAK_FINDER
    if (p != 0) {
        tc_allocation* allocation = find_free_allocation(&g_allocations);
        if (!allocation) {
            TYRAN_ERROR("Out of allocations!!! %zu", g_allocations.max_count);
            return 0;
        }
        allocation->is_allocated = 1;
        allocation->pointer = p;
        allocation->size = size;
        allocation->source_file = source_file;
        allocation->line_number = line_number;
    }
#else
    (void) source_file;
    (void) line_number;

#endif
    return p;
}

char* tc_str_dup(const char* str)
{
    size_t size = tc_strlen(str);
    char* mem = tc_malloc_type_count(char, size + 1);

    tc_strncpy(mem, size + 1, str, size);
    mem[size] = 0;
    return mem;
}

void tc_free_debug(void* p, const char* source_file, int line_number)
{
#if defined TC_MEM_LEAK_FINDER
    if (p != 0) {
        tc_allocation* allocation = find_allocation_from_pointer(&g_allocations, p);
        if (!allocation) {
            TYRAN_ERROR("Illegal free() not good!");
            return;
        }
        allocation->pointer = 0;
        allocation->is_allocated = 0;
        allocation->source_file = 0;
        allocation->line_number = 0;
        allocation->size = 0;
    }
#else
    (void) source_file;
    (void) line_number;

#endif
    free(p);
}

int tc_modulo(int a, int b)
{
    return (a % b + b) % b;
}


int tc_sign(int a)
{
    return (0 < a) - (a < 0);
}


int tc_min(int a, int b) {
    return a < b ? a : b;
}

int tc_max(int a, int b) {
    return a > b ? a : b;
}

/*

MIT License

Copyright (c) 2012 Peter Bjorklund

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
#include <tiny_libc/tiny_libc.h>

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
#endif
	return p;
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
#endif
	free(p);
}

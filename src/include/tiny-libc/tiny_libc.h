/*

MIT License

Copyright (c) 2017 Peter Bjorklund

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
#ifndef tc_h
#define tc_h

#include <memory.h>

#include <arpa/inet.h>
#include <stdlib.h>

#if defined TORNADO_OS_IOS
#include <string.h>
#endif

char* tc_str_dup(const char* str);

void tc_alloc_init(void);
void tc_alloc_reset(void);

void* tc_malloc_debug(size_t size, const char* source_file, int line_number);
void tc_free_debug(void* p, const char* source_file, int line_number);

#define tc_malloc(size) tc_malloc_debug(size, __FILE__, __LINE__)
#define tc_malloc_type(T) (T*) tc_malloc_debug(sizeof(T), __FILE__, __LINE__)
#define tc_malloc_type_count(T, N) (T*) tc_malloc_debug((N) * sizeof(T), __FILE__, __LINE__)

#define tc_free(p) tc_free_debug(p, __FILE__, __LINE__)

#define tc_memcpy_type(T, dest, source, N) memcpy(dest, source, (N) * sizeof(T))
#define tc_memcpy_type_n(dest, source, N) memcpy(dest, source, (N) * sizeof(*dest))
#define tc_memmove_octets memmove
#define tc_memmove_type(T, dest, source, N) memmove(dest, source, (N) * sizeof(T))

#define tc_memcpy_octets(dest, source, N) memcpy(dest, source, (N))

#define tc_memcmp memcmp
#define tc_memset_type(T, V) memset(T, V, sizeof(*T))
#define tc_memset_type_n(T, V, N) memset(T, V, sizeof(*T) * (N))
#define tc_mem_clear_type_n(T, N) tc_memset_type_n(T, 0, N);
#define tc_mem_clear_type(T) tc_memset_type(T, 0);
#define tc_mem_clear(D, N) memset(D, 0, N)

#define tc_htonl htonl
#define tc_htons htons
#define tc_ntohl ntohl
#define tc_ntohs ntohs

#if defined WIN32
// #pragma warning( disable : 4100 )

#define tc_sscanf sscanf_s
#define tc_snprintf sprintf_s
#define tc_strncpy(dest, dest_size, source, source_size) strncpy_s(dest, dest_size, source, source_size)
#define tc_strcpy(dest, dest_size, source) strcpy_s(dest, dest_size, source)
#define tc_fopen fopen_s
#define tc_fread fread
#define tc_fclose fclose
#define tc_strncat strncat_s
#define tc_strlen strlen

#else

#define tc_sscanf sscanf
#define tc_snprintf snprintf
#define tc_strncpy(dest, dest_size, source, source_size) strncpy(dest, source, source_size)
#define tc_strcpy(dest, dest_size, source) strcpy(dest, source)
#define tc_strpbrk strpbrk
#define tc_strncat strncat
#define tc_strlen strlen
#define tc_fopen(F, N, M) *F = fopen(N, M)
#define tc_fread fread
#define tc_fclose fclose
#define tc_strncat strncat

#endif

#define tc_strcmp strcmp
#define tc_strncmp strncmp
#define tc_strstr strstr
#define tc_fprintf fprintf
#define tc_fputs fputs
#define tc_fflush fflush
#define tc_strdup(str) tc_str_dup(str)
#define tc_str_equal(a, b) (tc_strcmp(a, b) == 0)
#define tc_str_chr(a, b) strchr(a, b)

#define tc_fmod fmod
#define tc_pow pow

#endif

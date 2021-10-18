/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef tc_h
#define tc_h

#include <memory.h>

#if defined TORNADO_OS_WINDOWS
#include <WinSock2.h>
#include <Windows.h>
#else
#include <arpa/inet.h>
#include <stdlib.h>
#endif

#if defined TORNADO_OS_IOS
#include <string.h>
#endif

char* tc_str_dup(const char* str);

void tc_alloc_init(void);
void tc_alloc_reset(void);

void* tc_malloc_debug(size_t size, const char* source_file, int line_number);
void tc_free_debug(void* p, const char* source_file, int line_number);
int tc_modulo(int a, int b);
int tc_sign(int a);
int tc_min(int a, int b);
int tc_max(int a, int b);

#define tc_malloc(size) tc_malloc_debug(size, __FILE__, __LINE__)
#define tc_malloc_type(T) (T*) tc_malloc_debug(sizeof(T), __FILE__, __LINE__)
#define tc_malloc_type_count(T, N) (T*) tc_malloc_debug((N) * sizeof(T), __FILE__, __LINE__)

#define tc_free(p) tc_free_debug(p, __FILE__, __LINE__)
#define tc_free_set_zero(p) tc_free_debug(p, __FILE__, __LINE__); p = 0
#define tc_memcpy_type(T, dest, source, N) memcpy(dest, source, (N) * sizeof(T))
#define tc_memcpy_type_n(dest, source, N) memcpy((void*) dest, source, (N) * sizeof(*dest))
#define tc_memmove_octets memmove
#define tc_memmove_type(T, dest, source, N) memmove(dest, source, (N) * sizeof(T))

#define tc_memcpy_octets(dest, source, N) memcpy(dest, source, (N))

#define tc_memcmp memcmp
#define tc_memset_type(T, V) memset(T, V, sizeof(*T))
#define tc_memset_type_n(T, V, N) memset(T, V, sizeof(*T) * (N))
#define tc_memset_octets(T, V, N) memset(T, V, N)
#define tc_mem_clear_type_n(T, N) tc_memset_type_n(T, 0, N)

#define tc_memchr(buf, ch, count) memchr(buf, ch, count)

//#define tc_mem_clear_type_array_n(T, N) memset(T, 0, sizeof(T) * N);

#define tc_mem_clear_type_array(P, N) memset( P, 0, sizeof(P[0]) * (N) )

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
#define tc_strncat strncat
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

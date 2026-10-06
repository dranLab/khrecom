#ifndef STRING_H
#define STRING_H

/* The C library's string functions that the game links (SN Systems libc), with the standard
 * signatures. size_t is unsigned int, as in the debug info. */

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned int size_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

void* memchr(const void* s, int c, size_t n);
int memcmp(const void* s1, const void* s2, size_t n);
void* memcpy(void* dest, const void* src, size_t n);
void* memmove(void* dest, const void* src, size_t n);
void* memset(void* s, int c, size_t n);

char* strcat(char* dest, const char* src);
char* strchr(const char* s, int c);
int strcmp(const char* s1, const char* s2);
char* strcpy(char* dest, const char* src);
size_t strlen(const char* s);
int strncmp(const char* s1, const char* s2, size_t n);
char* strncpy(char* dest, const char* src, size_t n);
char* strrchr(const char* s, int c);
char* strstr(const char* haystack, const char* needle);

int strcasecmp(const char* s1, const char* s2);
int strncasecmp(const char* s1, const char* s2, size_t n);

#ifdef __cplusplus
}
#endif

#endif /* STRING_H */

#ifndef _STRING_H
#define _STRING_H
#include "kernel/types.h"

char * itoa(char * str, int num);
void *memset(void* src, uint8_t value, uint32_t size);
void memcpy(void* dst_, const void* src_, uint32_t size);
char* strcpy(char* dst_, const char* src_);
uint32_t strlen(const char* str);
int8_t strcmp (const char *a, const char *b); 
int memcmp(const void * s1, const void *s2, int n);
char* strrchr(const char* str, int c);
char* strcat(char* strDest , const char* strSrc);
int strncmp ( char * s1, char * s2, int n);

#endif


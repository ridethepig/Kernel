#ifndef INCLUDE_STRING_H
#define INCLUDE_STRING_H

#include "types.h"

typedef unsigned char byte;

#define BYTE_COPY_FWD(dst_bp, src_bp, nbytes) \
	do                                          \
	{                                           \
		unsigned int __nbytes = (nbytes);         \
		while (__nbytes > 0)                      \
		{                                         \
			byte __x = ((byte *)src_bp)[0];         \
			src_bp += 1;                            \
			__nbytes -= 1;                          \
			((byte *)dst_bp)[0] = __x;              \
			dst_bp += 1;                            \
		}                                         \
	} while (0)

void *memcpy(void *dest, const void *src, size_t len);

void memset(void *dest, uint8_t val, size_t len);

void bzero(void *dest, size_t len);

int strcmp(const char *str1, const char *str2);

char *strcpy(char *dest, const char *src);

char *strcat(char *dest, const char *src);

int strlen(const char *src);

#endif

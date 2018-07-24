#include "string.h"

inline void *memcpy(void *dest, const void *src, size_t len)
{
    unsigned long int dstp = (long int)dest;
    unsigned long int srcp = (long int)src;
    BYTE_COPY_FWD(dstp, srcp, len);
    return dest;
}

inline void memset(void *dest, uint8_t val, size_t len)
{
    uint8_t *dst = (uint8_t *)dest;
    for (; len != 0; len --) {
        *dst ++ = val;
    }
}

inline void bzero(void *dest, size_t len)
{
    memset(dest, 0, len);
}

inline char *strcpy(char *dest, const char *src)
{
    return memcpy(dest, src, strlen(src) + 1);
}

inline char *strcat(char *dest, const char *src)
{
    strcpy(dest + strlen(dest), src);
    return dest;
}

inline int strlen(const char *str)
{
    const char *char_ptr;
    const unsigned long int *longword_ptr;
    unsigned long int longword, himagic, lomagic;
    for (char_ptr = str; ((unsigned long int)char_ptr & (sizeof(longword) - 1)) != 0; ++char_ptr)
        if (*char_ptr == '\0')
            return char_ptr - str;
    longword_ptr = (unsigned long int *)char_ptr;
    himagic = 0x80808080L;
    lomagic = 0x01010101L;
    if (sizeof(longword) > 4)
    {
        himagic = ((himagic << 16) << 16) | himagic;
        lomagic = ((lomagic << 16) << 16) | lomagic;
    }
    for (;;)
    {
        longword = *longword_ptr++;
        if (((longword - lomagic) & ~longword & himagic) != 0)
        {
            const char *cp = (const char *)(longword_ptr - 1);
            if (cp[0] == 0)
                return cp - str;
            if (cp[1] == 0)
                return cp - str + 1;
            if (cp[2] == 0)
                return cp - str + 2;
            if (cp[3] == 0)
                return cp - str + 3;
            if (sizeof(longword) > 4)
            {
                if (cp[4] == 0)
                    return cp - str + 4;
                if (cp[5] == 0)
                    return cp - str + 5;
                if (cp[6] == 0)
                    return cp - str + 6;
                if (cp[7] == 0)
                    return cp - str + 7;
            }
        }
    }
}

inline int strcmp(const char *str1, const char *str2)
{
    const unsigned char *s1 = (const unsigned char *)str1;
    const unsigned char *s2 = (const unsigned char *)str2;
    unsigned char c1, c2;
    do
    {
        c1 = (unsigned char)*s1++;
        c2 = (unsigned char)*s2++;
        if (c1 == '\0')
            return c1 - c2;
    } while (c1 == c2);
    return c1 - c2;
}

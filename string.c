#include "minicrt.h"

char* minicrt_itoa(int n, char* str, int radix)
{
    char digit[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char* p = str;
    char* head = str;
}

int minicrt_strcmp(const char* src, const char* dst)
{
    int ret = 0;
    char* p1 = (char *)src;
    char* p2 = (char *)dst;

    while (!(ret = (*p1 - *p2)) && *p1 && *p2) {
        ++p1;
        ++p2;
    }
    if (ret < 0)
        return -1;
    else if (ret > 0)
        return 1;
    else
        return 0;
}

char* minicrt_strcpy(char* dst, const char* src)
{
    char* ret = dst;
    while (*src)
        *dst++ = *src++;
    *dst = '\0';

    return ret;
}

int minicrt_strlen(const char* str)
{
    int cnt = 0;
    if (!str)
        return 0;
    for( ; *str != '\0'; ++str)
        ++cnt;

    return cnt;
}

void* minicrt_memcpy(void* dst, const void* src, int size)
{
    /*
     * The memory areas must not overlap.
     */
    asm("movl %1, %%esi\n\t"
            "movl %0, %%edi \n\t" 
            "movl %2, %%ecx \n\t"
            "cld \n\t"
            "rep movsb \n\t" : : "m"(dst), "m"(src), "m"(size));

    return dst;
}

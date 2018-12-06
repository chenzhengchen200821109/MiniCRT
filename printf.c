#include "minicrt.h"

int minicrt_fputc(int c, FILE* stream)
{
    if (minicrt_fwrite(&c, 1, 1, stream) != 1)
        return EOF;
    else
        return c;
}

int minicrt_fputs(const char* str, FILE* stream)
{
    int len = minicrt_strlen(str);
    if (minicrt_fwrite(str, 1, len, stream) != len)
        return EOF;
    else
        return len;
}

#define va_list char*
#define va_start(ap, arg) (ap = (va_list)&arg + sizeof(arg))
#define va_arg(ap, t) (*(t*)((ap += sizeof(t))-sizeof(t)))
#define va_end(ap) (ap = (va_list)0)

int vfprintf(FILE* stream, const char* format, va_list arglist)
{
    int translating = 0;
    int ret = 0;
    const char* p = 0;
    for (p = format; *p != '\0'; ++p) {
        switch (*p) {
            case '%':
                if (!translating)
                    translating = 1;
                else {
                    if (minicrt_fputc('%', stream) < 0)
                        return EOF;
                    ++ret;
                    translating = 0;
                }
                break;
            case 'd':
                if (translating) {
                    char buf[6];
                    translating = 0;
                    minicrt_itoa(va_arg(arglist, int), buf, 10);
                    if (minicrt_fputs(buf, stream) < 0)
                        return EOF;
                    ret += minicrt_strlen(buf);
                } else if (minicrt_fputc('d', stream) < 0)
                    return EOF;
                else
                    ++ret;
                break;
            case 's':
                if (translating) {
                    const char* str = va_arg(arglist, const char *);
                    translating = 0;
                    if (minicrt_fputs(str, stream) < 0)
                        return EOF;
                    ret += minicrt_strlen(str);
                } else if (minicrt_fputc('s', stream) < 0)
                    return EOF;
                else
                    ++ret;
                break;
            default:
                if (translating)
                    translating = 0;
                if (minicrt_fputc(*p, stream) < 0)
                    return EOF;
                else
                    ++ret;
                break;
        }
    }
    return ret;
}

int printf(const char* format, ...)
{
    va_list(arglist);
    va_start(arglist, format);
    return vfprintf(stdout, format, arglist);
}

int fprintf(FILE* stream, const char* format, ...)
{
    va_list(arglist);
    va_start(arglist, format);
    return vfprintf(stream, format, arglist);
}

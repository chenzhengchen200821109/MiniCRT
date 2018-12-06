#include "minicrt.h"

int __sdidinit;

int minicrt_io_init()
{
    return 1;
}

/*
 * Wrapper function for system call open().
 */
static int minicrt_open(const char* pathname, int flags, int mode)
{
    int fd = 0;
    asm("movl $5, %%eax \n\t"
            "movl %1, %%ebx \n\t"
            "movl %2, %%ecx \n\t"
            "movl %3, %%edx \n\t"
            "int $0x80 \n\t"
            "movl %%eax, %0 \n\t" :
            "=m"(fd) : "m"(pathname), "m"(flags), "m"(mode));
}

/*
 * Wrapper function for system call read().
 */
static int minicrt_read(int fd, void* buffer, unsigned int size)
{
    int ret = 0;
    asm("movl $3, %%eax \n\t"
            "movl %1, %%ebx \n\t"
            "movl %2, %%ecx \n\t"
            "movl %3, %%edx \n\t"
            "int $0x80 \n\t"
            "movl %%eax, %0 \n\t" :
            "=m"(ret) : "m"(fd), "m"(buffer), "m"(size));
    return ret;
}

static int minicrt_write(int fd, const void* buffer, unsigned int size)
{
    int ret = 0;
    asm("movl $4, %%eax \n\t"
            "movl %1, %%ebx \n\t"
            "movl %2, %%ecx \n\t"
            "movl %3, %%edx \n\t"
            "int $0x80 \n\t"
            "movl %%eax, %0 \n\t" :
            "=m"(ret) : "m"(fd), "m"(buffer), "m"(size));
    return ret;
}

static int minicrt_close(int fd)
{
    int ret = 0;
    asm("movl $6, %%eax \n\t"
            "movl %1, %%ebx \n\t"
            "int $0x80 \n\t"
            "movl %%eax, %0 \n\t" :
            "=m"(ret) : "m"(fd));
    return ret;
}

static int minicrt_seek(int fd, int offset, int mode)
{
    int ret = 0;
    asm("movl $19, %%eax \n\t"
            "movl %1, %%ebx \n\t"
            "movl %2, %%ecx \n\t"
            "movl %3, %%edx \n\t"
            "int $0x80 \n\t"
            "movl %%eax, %0 \n\t" :
            "=m"(ret) : "m"(fd), "m"(offset), "m"(mode));
    return ret;
}

void __sinit()
{
    __sdidinit = 1;
}

FILE* minicrt_fopen(const char* filename, const char* mode)
{
    int fd = -1;
    int flags = 0;
    int access = 00700;

#define O_RDONLY    00
#define O_WRONLY    01
#define O_RDWR      02
#define O_CREAT     0100
#define O_TRUNC     01000
#define O_APPEND    02000

    if (minicrt_strcmp(mode, "w") == 0)
        flags |= O_WRONLY | O_CREAT | O_TRUNC;
    if (minicrt_strcmp(mode, "w+") == 0)
        flags |= O_RDWR | O_CREAT | O_TRUNC;
    if (minicrt_strcmp(mode, "r") == 0)
        flags |= O_RDONLY;
    if (minicrt_strcmp(mode, "r+") == 0)
        flags |= O_RDWR | O_CREAT;
    fd = minicrt_open(filename, flags, access);
    return (FILE *)fd;
}

int minicrt_fread(void* buffer, int size, int count, FILE* stream)
{
    return minicrt_read((int)stream, buffer, size * count);
}

int minicrt_fwrite(const void* buffer, int size, int count, FILE* stream)
{
    return minicrt_write((int)stream, buffer, size * count);
}

int minicrt_fclose(FILE* fp)
{
    return minicrt_close((int)fp);
}

int minicrt_fseek(FILE* fp, int offset, int set)
{
    return minicrt_seek((int)fp, offset, set);
}



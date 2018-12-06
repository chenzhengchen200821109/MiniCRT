#ifndef _STDIO_H_
#define _STDIO_H_

#include "types.h"


#define DEFAULT_FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)
#define SIZE_MAX UINT_MAX

/*
 * based on implementation of FreeBSD.
 */

/* stdio buffers */
struct __sbuf
{
    unsigned char* _base;
    int _size;
};
struct __sFILE
{
    unsigned char* _p;           // current position in buffer
    int _r;                      // read space left
    int _w;                      // write space left
    int _flags;                  // flags
    int _file;                   // fileno, if Unix descriptor
    struct __sbuf _bf;           // the buffer (at least 1 byte, if !NULL)
    int _offset;                 // current lseek offset
};

typedef struct __sFILE FILE;

/*
 * I/O descriptor for __sfvwrite().
 */
struct __siov
{
    void* iov_base;
    int iov_len;
};

struct __suio
{
    struct __siov* uio_iov;
    int uio_iovcnt;
    int uio_resid;
};

#endif

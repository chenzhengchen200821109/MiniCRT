#ifndef _ATEXIT_H
#define _ATEXIT_H

typedef void (*cxa_func_t)(void *);
typedef void (*atexit_func_t)(void);
int __cxa_atexit(cxa_func_t func, void* arg, void *);
int atexit(atexit_func_t func);

#endif

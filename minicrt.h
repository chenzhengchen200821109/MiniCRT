#ifndef _MINICRT_H_
#define _MINICRT_H_

#ifdef __cplusplus
extern "C" {
#endif

// malloc
#ifndef NULL
#define NULL (0)
#endif

void minicrt_free(void*);
void* minicrt_malloc(unsigned int size);
static int brk(void* end_data_segment);
int minicrt_heap_init();

// strings
char* minicrt_itoa(int n, char* str, int radix);
int minicrt_strcmp(const char* src, const char* dst);
char* minicrt_strcpy(char* dest, const char* src);
int minicrt_strlen(const char* str);

void* minicrt_memcpy(void* dest, const void* src, int size);

// file and io
typedef int FILE;

#define EOF (-1)
#define stdin ((FILE *)0)
#define stdout ((FILE *)0)
#define stderr ((FILE *)0)

int minicrt_io_init();
FILE* minicrt_fopen(const char* filename, const char* mode);
int minicrt_fread(void* buffer, int size, int count, FILE* stream);
int minicrt_fwrite(const void* buffer, int size, int count, FILE* stream);
int minicrt_fclose(FILE* fp);
int minicrt_fseek(FILE* fp, int offset, int set);

// printf
int minicrt_fputc(int c, FILE* stream);
int minicrt_fputs(const char* str, FILE* stream);
int minicrt_printf(const char* format, ...);
int minicrt_fprintf(FILE* stream, const char* format, ...);

// internal
void minicrt_do_global_ctors();
void minicrt_call_exit_routine();

// atexit
typedef void (*atexit_func_t)(void);
int atexit(atexit_func_t func);

#ifdef __cplusplus
}
#endif

#endif // _MINICRT_H_

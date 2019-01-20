#include "minicrt.h"

extern int main(int argc, char* argv[]);
void minicrt_exit(int);

static void minicrt_fatal_error(const char* msg)
{
    // printf("fatal error: %s", msg);
    minicrt_exit(1);
}

void minicrt_entry(void)
{
    int ret;
    int argc;
    char** argv;

    char* ebp_reg = 0;
    // ebp_reg = %ebp
    asm("movl %%ebp, %0 \n" : "=r"(ebp_reg));

    argc = *(int *)(ebp_reg + 4);
    argv = (char **)(ebp_reg + 8);

    if (!minicrt_heap_init())
        minicrt_fatal_error("Heap initializations failed");

    if (!minicrt_io_init())
        minicrt_fatal_error("IO initializations failed");

    minicrt_do_global_ctors();

    ret = main(argc, argv);
    minicrt_exit(ret);
}

void minicrt_exit(int code)
{
    minicrt_call_exit_routine();
    __asm__ volatile (
            "movl %0, %%ebx \n" 
            "movl $1, %%eax \n" 
            "int $0x80 \n" 
            "hlt \n" : "=m"(code));
}

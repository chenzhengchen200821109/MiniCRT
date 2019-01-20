
typedef unsigned int size_t;

extern "C" void* minicrt_malloc(size_t t);
extern "C" void minicrt_free(void* );

void* operator new(size_t size)
{
    return minicrt_malloc(size);
}

void operator delete(void* p)
{
    minicrt_free(p);
}

void* operator new[](size_t size)
{
    return minicrt_malloc(size);
}

void operator delete[](void* p)
{
    minicrt_free(p);
}

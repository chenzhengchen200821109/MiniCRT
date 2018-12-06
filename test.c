#include "minicrt.h"

int main(int argc, char* argv[])
{
    int i;
    FILE* fp;
    char** v = minicrt_malloc(argc * sizeof(char *));
    for (i = 0; i < argc; ++i) {
        v[i] = minicrt_malloc(minicrt_strlen(argv[i]) + 1);
        minicrt_strcpy(v[i], argv[i]);
    }

    fp = minicrt_fopen("test.txt", "w");
    for (i = 0; i < argc; ++i) {
        int len = minicrt_strlen(v[i]);
        minicrt_fwrite(&len, 1, sizeof(int), fp);
        minicrt_fwrite(v[i], 1, len, fp);
    }
    minicrt_fclose(fp);

    fp = minicrt_fopen("test.txt", "r");
    for (i = 0; i < argc; ++i) {
        int len;
        char* buf;
        minicrt_fread(&len, 1, sizeof(int), fp);
        buf = minicrt_malloc(len + 1);
        minicrt_fread(buf, 1, len, fp);
        buf[len] = '\0';
        minicrt_printf("%d %s\n", len, buf);
        minicrt_free(buf);
        minicrt_free(v[i]);
    }
    minicrt_fclose(fp);
}

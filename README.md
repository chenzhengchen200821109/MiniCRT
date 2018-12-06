# How to use MiniCRT  
gcc -c -ggdb -fno-builtin -nostdlib -fno-stack-protector test.c  
ld -static -e minicrt_entry entry.o test.o mincrt.a -o test

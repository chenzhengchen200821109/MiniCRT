
all: minicrt.a entry.o

minicrt.a: malloc.o printf.o stdio.o string.o 
	ar -rs minicrt.a malloc.o printf.o stdio.o string.o

entry.o: entry.c
	gcc -c -fno-builtin -nostdlib -fno-stack-protector entry.c

malloc.o: malloc.c
	gcc -c -fno-builtin -nostdlib -fno-stack-protector malloc.c

stdio.o: stdio.c
	gcc -c -fno-builtin -nostdlib -fno-stack-protector stdio.c

string.o: string.c
	gcc -c -fno-builtin -nostdlib -fno-stack-protector string.c

printf.o: printf.c
	gcc -c -fno-builtin -nostdlib -fno-stack-protector printf.c

clean:
	rm minicrt.a malloc.o printf.o stdio.o string.o entry.o


gcc -fno-stack-protector -w -c main.c
gcc -fno-stack-protector -w -c bus.c
gcc -fno-stack-protector -w -c dllist.c
gcc -fno-stack-protector -w -c jval.c
gcc -fno-stack-protector -w -c jrb.c
gcc main.o bus.o dllist.o jval.o jrb.o -o result
./result
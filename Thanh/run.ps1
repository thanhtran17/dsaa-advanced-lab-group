gcc -c 1.c
gcc -c dllist.c
gcc -c jrb.c
gcc -c jval.c
gcc -o 1 1.o dllist.o jrb.o jval.o
./1
gcc -c -Wall jval.c
gcc -c jrb.c 
gcc -c 1.c
gcc -c dllist.c
gcc 1.o dllist.o jval.o jrb.o -o 1
./1
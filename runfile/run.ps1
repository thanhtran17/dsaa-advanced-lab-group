gcc -c main.c
gcc -c bus.c
gcc -c dllist.c
gcc -c jval.c
gcc -c jrb.c
gcc main.o bus.o dllist.o jval.o jrb.o -o result
./result
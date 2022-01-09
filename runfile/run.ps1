gcc -c main.c
gcc -c BusMap.c
gcc -c dllist.c
gcc -c jval.c
gcc -c jrb.c
gcc main.o BusMap.o dllist.o jval.o jrb.o -o result
./result
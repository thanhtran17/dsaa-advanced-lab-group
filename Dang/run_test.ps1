gcc -c -Wall jval.c
gcc -c jrb.c 
gcc -c test.c
gcc -c dllist.c
gcc test.o dllist.o jval.o jrb.o -o graph
./graph
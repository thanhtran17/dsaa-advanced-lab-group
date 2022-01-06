gcc -c -Wall jval.c
gcc -c jrb.c 
gcc -c practice_2.c
gcc -c dllist.c
gcc practice_2.o dllist.o jval.o jrb.o -o graph
./graph
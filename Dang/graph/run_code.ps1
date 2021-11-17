gcc -c -Wall jval.c
gcc -c -Wall queue.c
gcc -c -Wall stack.c
gcc -c jrb.c 
gcc -c "dfsbfs.c"
gcc "dfsbfs.o" jval.o jrb.o stack.o queue.o -o graph
./graph
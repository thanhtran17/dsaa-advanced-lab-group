gcc -c -Wall jval.c
gcc -c -Wall queue.c
gcc -c -Wall stack.c
gcc -c jrb.c 
gcc -c "quiz2.c"
gcc "quiz2.o" jval.o jrb.o stack.o queue.o -o graph
./graph
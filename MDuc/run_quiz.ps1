gcc -c -Wall jval.c
gcc -c jrb.c 
gcc -c quiz1.c
gcc -c dllist.c
gcc -c stack.c
gcc quiz1.o dllist.o jval.o stack.o jrb.o -o graph
./graph
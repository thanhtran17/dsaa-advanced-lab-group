gcc -c -Wall jval.c
gcc -c jrb.c 
gcc -c quiz.c
gcc -c dllist.c
gcc -c stack.c
gcc quiz.o dllist.o jval.o stack.o jrb.o -o graph
./graph
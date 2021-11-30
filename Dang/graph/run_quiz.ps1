gcc -c -Wall jval.c
gcc -c jrb.c 
gcc -c quiz.c
gcc quiz.o jval.o jrb.o -o graph
./graph
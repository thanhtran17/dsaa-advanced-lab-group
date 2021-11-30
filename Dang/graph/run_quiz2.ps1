gcc -c -Wall jval.c
gcc -c jrb.c 
gcc -c "quiz2.c"
gcc "quiz2.o" jval.o jrb.o -o graph
./graph
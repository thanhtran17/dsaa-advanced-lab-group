gcc -c -Wall jval.c
gcc -c jrb.c 
gcc -c "quiz1.c"
gcc "quiz1.o" jval.o jrb.o -o graph
./graph
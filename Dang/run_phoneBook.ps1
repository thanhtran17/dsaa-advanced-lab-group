gcc -c -Wall jval.c
gcc -c jrb.c 
gcc -c phoneBook.c
gcc phoneBook.o jval.o jrb.o -o phoneBook
./phoneBook
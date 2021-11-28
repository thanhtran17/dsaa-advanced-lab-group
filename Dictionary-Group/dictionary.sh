rm -f test
gcc -Wall -c soundex.c
gcc -c dictionary.c `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0` -Lbtree/lib -lbt -Ibtree/inc 
gcc dictionary.o soundex.o -o test `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0` -Lbtree/lib -lbt -Ibtree/inc 
./test


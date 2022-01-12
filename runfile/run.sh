gcc -fno-stack-protector -w -c bus.c
gcc -fno-stack-protector -w -c dllist.c
gcc -fno-stack-protector -w -c jval.c
gcc -fno-stack-protector -w -c jrb.c
gcc -w -c dictionary.c `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`
gcc dictionary.o bus.o dllist.o jval.o jrb.o `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0` -o result
./result
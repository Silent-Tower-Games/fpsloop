CC=gcc

#SRC = $(wildcard ./src/*.c)
#OBJS = $(patsubst %.c, %.o, $(SRC))

application:
	make lib
	make objs
	${CC} ./src/test.o -o main `sdl2-config --libs` -L`pwd` -lfpsloop -Wl,-rpath=./

application-static:
	make lib-static
	make objs
	${CC} ./src/test.o libfpsloop.a -o main `sdl2-config --libs`

lib:
	make objs-lib
	${CC} ./src/FPSLoop.o -shared -o libfpsloop.so `sdl2-config --libs`

lib-static:
	make objs-lib
	ar rcs libfpsloop.a src/FPSLoop.o

objs:
	gcc -c ./src/test.c -o ./src/test.o

objs-lib:
	gcc -c ./src/FPSLoop.c -o ./src/FPSLoop.o -fPIC

clear:
	rm -f ./src/*.o ./main ./libfpsloop.so

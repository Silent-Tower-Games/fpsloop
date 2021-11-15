CC=gcc
CFLAGS=-pedantic -O3

application:
	make lib
	make objs
	${CC} ${CFLAGS} ./src/test.o -o main `sdl2-config --libs` -L`pwd` -lfpsloop -Wl,-rpath=./

application-static:
	make lib-static
	make objs
	${CC} ${CFLAGS} ./src/test.o libfpsloop.a -o main `sdl2-config --libs`

lib:
	make objs-lib
	${CC} ${CFLAGS} ./src/FPSLoop.o -shared -o libfpsloop.so `sdl2-config --libs`

lib-static:
	make objs-lib
	ar rcs libfpsloop.a src/FPSLoop.o

objs:
	${CC} ${CFLAGS} -c ./src/test.c -o ./src/test.o

objs-lib:
	${CC} ${CFLAGS} -c ./src/FPSLoop.c -o ./src/FPSLoop.o -fPIC

clear:
	rm -f ./src/*.o ./main ./libfpsloop.so

valgrind:
	valgrind ./main -v --leak-check=full 2> valgrind.txt

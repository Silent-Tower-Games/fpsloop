CC?=gcc
EXT?=so
RPATH?=-Wl,-rpath=./

ifeq (${CC}, clang)
RPATH=-Wl,-rpath ./
endif

.PHONY=application
application:
	make lib
	make objs
	${CC} ${CFLAGS} ./src/test.o -o main ${LIBS} -lfpsloop ${RPATH}

.PHONY=application-static
application-static:
	make lib-static
	make objs
	${CC} ${CFLAGS} ./src/test.o libfpsloop.a -o main ${LIBS}

.PHONY=lib
lib:
	make objs-lib
	${CC} ${CFLAGS} ./src/FPSLoop.o -shared -o libfpsloop.${EXT} ${LIBS}

.PHONY=lib-static
lib-static:
	make objs-lib
	ar rcs libfpsloop.a src/FPSLoop.o

.PHONY=objs
objs:
	${CC} ${CFLAGS} -c ./src/test.c -o ./src/test.o ${INCS}

.PHONY=objs-lib
objs-lib:
	${CC} ${CFLAGS} -c ./src/FPSLoop.c -o ./src/FPSLoop.o -fPIC ${INCS} ${LIBS}

.PHONY=clean
clean:
	rm -f ./src/*.o ./main ./libfpsloop.${EXT}

.PHONY=valgrind
valgrind:
	valgrind --leak-check=full ./main 2> valgrind.txt

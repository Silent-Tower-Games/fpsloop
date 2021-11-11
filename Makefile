CC=gcc

# Got this from a blog post
# https://blog.jgc.org/2011/07/gnu-make-recursive-wildcard-function.html
rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

SRC = $(call rwildcard ,,*.c)
OBJS = $(patsubst %.c, %.o, $(SRC))

application:
	make objs
	${CC} ${OBJS} -o main `sdl2-config --libs`

objs: $(OBJS)

clear:
	rm -f ./src/*.o ./main

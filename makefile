#Name: Nate Stewart
#Date: 08-19-15
COMPILE_PARAMS := -O2 -Werror -Wall -g -c
LINK_PARAMS    := -lm

all: life

life: life.o
	gcc $(LINK_PARAMS) -o life life.o

life.o: life.c life.h
	gcc $(COMPILE_PARAMS) -o life.o life.c

clean:
	rm -f *.o life


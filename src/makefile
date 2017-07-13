NAME = main
EXTRA_CFLAGS += -Wall -Werror -g 
LIB += -L. -ldlist

$(NAME): libdlist.a test.c dlist.h
	gcc $(EXTRA_CFLAGS) -o $(NAME) test.c $(LIB)

libdlist.a: dlist.o
	ar cr libdlist.a dlist.o

dlist.o: dlist.c 
	gcc -c dlist.c

.PHONY: clean
clean:
	-rm *.o 

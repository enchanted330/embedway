NAME = ls
LIB += -L. -ldlist
$(NAME): myls.o libdlist.a dlist.h
	$(CC) -Wall -g -o $(NAME) myls.o $(LIB)
libdlist.a: dlist.o
	$(AR) cr libdlist.a dlist.o
myls.o:
	$(CC) -c myls.c
dlist.o:
	$(CC) -c dlist.c
.PHONY: clean
clean:
	-rm *.o $(NAME)



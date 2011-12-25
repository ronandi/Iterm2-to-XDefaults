CC	=	gcc
COMPILE	=	$(CC)
CFLAGS	=	-Wall -g
all: itox 
itox: converttox.c 
	$(COMPILE) $(CFLAGS) -o itox converttox.c  
clean:
	rm -rf itox 
.PHONY: clean	all

BUILD_DIR = $(shell pwd)
SRC = ../src
CC ?= gcc
CFLAG:=-g 
CFLAG+=-Wall
CFLAG+=-fsanitize=address

LIB = ../coroutine.a

check:
	@echo "chekc is running"
	$(CC) -o test test_coroutine.c $(LIB)  $(CFLAG)

clone:
	@echo "clone is running"
	$(CC) -o test test_clone.c $(LIB) $(CFLAG)

indent:
	@echo "indent is running"
	clang-format -i *.[ch]	

clean:
	@echo "clean is running"
	rm -f test
	rm -rf test.dSYM

rq:
	@echo "rq is running"
	$(CC) -o test test_fifo_runqueue.c $(LIB) $(CFLAG)
stack:
	@echo "stack is running"
	$(CC) -o test test_stack.c $(LIB) $(CFLAG)


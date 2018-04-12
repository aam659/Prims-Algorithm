DLLOBJS = integer.o real.o string.o dll.o test-dll.o
SLLOBJS = integer.o real.o string.o sll.o test-sll.o
SOBJS = integer.o real.o string.o dll.o stack.o test-stack.o
QOBJS = integer.o real.o string.o sll.o queue.o test-queue.o
BOBJS = integer.o real.o string.o dll.o binomial.o binomial-0-0.o
BSTOBJS = integer.o real.o string.o sll.o queue.o bst.o test-bst.o
OOPTS = -Wall -Wextra -std=c99 -g -c
EOPTS = -Wall -Wextra -std=c99 -g

all : test-dll test-sll test-stack test-queue test-bst test-binomial0

test-dll : $(DLLOBJS)
	gcc $(EOPTS) -o test-dll $(DLLOBJS)

test-sll : $(SLLOBJS)
	gcc $(EOPTS) -o test-sll $(SLLOBJS)

test-stack : $(SOBJS)
	gcc $(EOPTS) -o test-stack $(SOBJS)

test-queue : $(QOBJS)
	gcc $(EOPTS) -o test-queue $(QOBJS)

test-bst : $(BSTOBJS)
	gcc $(EOPTS) -o test-bst $(BSTOBJS)

test-binomial0 : $(BOBJS)
	gcc $(EOPTS) -o test-binomial0 $(BOBJS)

dll.o : dll.c dll.h
	gcc $(OOPTS) dll.c

sll.o : sll.c sll.h
	gcc $(OOPTS) sll.c

stack.o : stack.c stack.h dll.h
	gcc $(OOPTS) stack.c

queue.o : queue.c queue.h sll.h
	gcc $(OOPTS) queue.c

integer.o : integer.c integer.h
	gcc $(OOPTS) integer.c

real.o : real.c real.h
	gcc $(OOPTS) real.c

string.o : string.c string.h
	gcc $(OOPTS) string.c

scanner.o : scanner.c scanner.h
	gcc $(OOPTS) scanner.c

bst.o : bst.c bst.h
	gcc $(OOPTS) bst.c

avl.o : avl.c integer.h real.h string.h sll.h queue.h bst.h avl.h
	gcc $(OOPTS) avl.c

binomial.o : binomial.c binomial.h dll.h
	gcc $(OOPTS) binomial.c

test-dll.o : test-dll.c integer.h dll.h
	gcc $(OOPTS) test-dll.c

test-sll.o : test-sll.c integer.h sll.h
	gcc $(OOPTS) test-sll.c

test-stack.o : test-stack.c integer.h dll.h stack.h
	gcc $(OOPTS) test-stack.c

test-queue.o : test-queue.c integer.h sll.h queue.h
	gcc $(OOPTS) test-queue.c

binomial-0-0.o : binomial-0-0.c integer.h real.h string.h dll.h
	gcc $(OOPTS) binomial-0-0.c

test-bst.o : test-bst.c integer.h real.h string.h sll.h queue.h
	gcc $(OOPTS) test-bst.c

valgrind : all
	@echo valgrind doubly-linked list
	valgrind ./test-dll
	@echo valgrind singly-linked list
	valgrind ./test-sll
	@echo
	@echo valgrind doubly-linked list
	valgrind ./test-dll
	@echo
	@echo valgrind stack
	valgrind ./test-stack
	@echo
	@echo valgrind queue
	valgrind ./test-queue
	@echo valgrind bst
	valgrind ./test-bst
	@echo valgrind binomial heap
	valgrind ./test-binomial0

test : all
	@echo testing doubly-linked list
	./test-dll
	@echo testing singly-linked list
	./test-sll
	@echo
	@echo
	@echo testing stack
	./test-stack
	@echo
	@echo testing queue
	./test-queue
	@echo

clean   :
	rm -f *.o test-dll test-sll test-stack test-queue test-bst test-binomial0



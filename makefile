DLLOBJS = integer.o dll.o test-dll.o
#SLLOBJS = integer.o sll.o test-sll.o
#SOBJS = integer.o dll.o stack.o test-stack.o
#QOBJS = integer.o sll.o queue.o test-queue.o
OOPTS = -Wall -Wextra -std=c99 -g -c
EOPTS = -Wall -Wextra -std=c99 -g

all : test-dll 
#test-sll test-stack test-queue

test-dll : $(DLLOBJS)
	gcc $(EOPTS) -o test-dll $(DLLOBJS)

#test-sll : $(SLLOBJS)
#	gcc $(EOPTS) -o test-sll $(SLLOBJS)

#test-stack : $(SOBJS)
#	gcc $(EOPTS) -o test-stack $(SOBJS)

#test-queue : $(QOBJS)
#	gcc $(EOPTS) -o test-queue $(QOBJS)

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

test-dll.o : test-dll.c integer.h dll.h
	gcc $(OOPTS) test-dll.c

#test-sll.o : test-sll.c integer.h sll.h
#	gcc $(OOPTS) test-sll.c

#test-stack.o : test-stack.c integer.h dll.h stack.h
#	gcc $(OOPTS) test-stack.c

#test-queue.o : test-queue.c integer.h sll.h queue.h
#	gcc $(OOPTS) test-queue.c

valgrind : all
	echo testing doubly-linked list
	valgrind ./test-dll
#	echo testing singly-linked list
#	valgrind ./test-sll
#	echo
#	echo testing doubly-linked list
#	valgrind ./test-dll
#	echo
#	echo testing stack
#	valgrind ./test-stack
#	echo
#	echo testing queue
#	valgrind ./test-queue
#	echo

test : all
	echo testing doubly-linked list
	./test-dll
#	echo testing singly-linked list
#	./test-sll
#	echo
#	echo
#	echo testing stack
#	./test-stack
#	echo
#	echo testing queue
#	./test-queue
	echo

clean   :
	rm -f *.o test-dll #test-sll test-stack test-queue



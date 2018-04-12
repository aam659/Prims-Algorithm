/* Author: Alan Manning */
/* Class: CS201 Data Structures and Algorithms */
/* Assignment 0 */
/* 7 September, 2017 */

/* This is the test file for the header files for assignment 0.
The data types that were used as tests consisted of doubles, ints, chars, etc,
where these types came from user-defined classes. */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "sll.h"
#include "integer.h"
#include "queue.h"
#include "string.h"
#include "real.h"

static void showItems(QUEUE *items) {
	printf("The items are ");
	displayQUEUE(items, stdout);				// Calls displaySTACK method
	printf(".\n");
	printf("The items (debugged) are ");
	displayQUEUEdebug(items, stdout);
	printf(".\n");

	return;
}

static void sizeCall(QUEUE *items) {
	printf("sizeSTACK method call: ");
	int size = sizeQUEUE(items);				// Calls sizeSTACK method
	printf("%d\n", size);

	return;
}

int main(int argc, char **argv) {
	int i;
	if (argc != 1) {
		fprintf(stderr, "usage: %s\n", argv[0]);	// Error if argc != 1
		exit(1);
	}

	QUEUE *items = newQUEUE(displayINTEGER,freeINTEGER);	// Creates queue object
	showItems(items);					// Prints empty queue details
	sizeCall(items);
	printf("enqueue (5): ");
	enqueue(items, newINTEGER(5));				// Enqueues 5
	printf("\n");						// Prints details
	sizeCall(items);
	showItems(items);

	printf("peekSTACK method call (INTEGER): ");
	int x = getINTEGER((INTEGER *) peekQUEUE(items));	// Tests peekQUEUE method-top of queue is 5
	printf("\nThe item at the top of the queue is %d.\n", x);
	printf("enqueue method call for 6, 2, 9, and 1: ");
	enqueue(items, newINTEGER(6));				// Enqueues 6, 2, 9, and 1
	enqueue(items, newINTEGER(2));
	enqueue(items, newINTEGER(9));
	enqueue(items, newINTEGER(1));
	printf("\n");						// Prints resulting information
	sizeCall(items);
	showItems(items);
	printf("peekQUEUE method call (INTEGER): ");		
	x = getINTEGER((INTEGER *) peekQUEUE(items));		// Shows top of queue (5)
	printf("\nThe item at the top of the queue is %d.\n", x);
	printf("pop method call: ");
 	printf("The value ");
	INTEGER *y = dequeue(items);
    	displayINTEGER(y,stdout);				// Tests dequeue and returns 1
    	printf(" was removed.\n");
	freeINTEGER(y);
    	showItems(items);
	sizeCall(items);
	printf("peekSTACK method call (INTEGER): ");		// Shows new top of queue (6)
	x = getINTEGER((INTEGER *) peekQUEUE(items));
	printf("\nThe item at the top of the stack is %d.\n", x);
	printf("REMOVING ALL ITEMS FROM STACK:\n");

	int stackSize = sizeQUEUE(items);
	INTEGER *z = 0;
	for (i = 0; i < stackSize; ++i) {			// Removes remaining items
		printf("The value ");
		z = dequeue(items);
		displayINTEGER(z,stdout);
		printf(" was removed.\n");

		freeINTEGER(z);
	}
	showItems(items);					// Prints details for empty queue 
	sizeCall(items);


	printf("Inserting many values into stack!\n");

	for (i = 0; i < 10000; ++i) {				// Inserts 1000000 items
		enqueue(items, newINTEGER(2 * i));
	}	

	showItems(items);					// Prints details of queue
	sizeCall(items);

	printf("peekQUEUE method call (INTEGER): ");		// Shows top of queue
	x = getINTEGER((INTEGER *) peekQUEUE(items));
	printf("\n the item at the top of the stack is %d.\n", x);

	printf("Emptying queue!\n");
	
	INTEGER *w = 0;

	for (i= 10000; i > 0; --i) {				// Empties stack
		w = dequeue(items);

		freeINTEGER(w);
	}

	showItems(items);					// Prints details of empty queue 
	sizeCall(items);
	freeQUEUE(items);

	return 0;
}




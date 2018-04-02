#include <stdio.h>
#include <stdlib.h>
#include "dll.h"
#include "integer.h"
#include "string.h"
#include "real.h"

static void showItems(DLL *items) {
	printf("The items are ");
       	displayDLL(items,stdout);
       	printf(".\n");
       	printf("The items (debugged) are ");
       	displayDLLdebug(items,stdout);
       	printf(".\n");
}

int main(int argc,char **argv) {
	int count = 0;

	if (argc != 1) {
        	fprintf(stderr,"usage: %s\n",argv[0]);
        	exit(1);
        }

       	DLL *items = newDLL(displayINTEGER,freeINTEGER);
	DLL *secondItems = newDLL(displayINTEGER,freeINTEGER);
	DLL *thirdItems = newDLL(displayINTEGER,freeINTEGER);

       	showItems(items);
       	insertDLL(items,0,newINTEGER(3));                   	//insert at front
	insertDLL(items,0,newINTEGER(4));
       	insertDLL(items,sizeDLL(items),newINTEGER(2));      	//insert at back
       	insertDLL(items,1,newINTEGER(1));                   	//insert at middle
       	showItems(items);
       	printf("The value ");
       	INTEGER *i = removeDLL(items,0);                   	//remove from front
       	displayINTEGER(i,stdout);
       	printf(" was removed.\n");
       	freeINTEGER(i);
       	showItems(items);
       	int x = getINTEGER((INTEGER *) getDLL(items,0));    	//get the first item
       	printf("The first item is %d.\n",x);
//	INTEGER *j = newINTEGER(7);
//	setDLL(items, 0, j);
//	INTEGER *k = removeDLL(items,0);
//	printf("The first item is now ");
//	displayINTEGER(k,stdout);
//	printf("\n");
//	freeINTEGER(k);
	showItems(items);
	printf("The size of the list is %d\n", sizeDLL(items));
       	printf("Freeing the list.\n");
       	freeDLL(items);

	printf("Printing the second list:\n");
	showItems(secondItems);

	printf("Printing the third list:\n");
	showItems(thirdItems);

	for (count = 0; count < 64; ++count) {
		insertDLL(secondItems,0,newINTEGER(count));
		insertDLL(thirdItems,0,newINTEGER(2 * count));
	}

	printf("Second list after 64 inserts:\n");
	showItems(secondItems);

	printf("Third list after 64 inserts:\n");
	showItems(thirdItems);

	printf("Inserting 0 into 32nd element of second list:\n");
	insertDLL(secondItems,(sizeDLL(secondItems) / 2),newINTEGER(0));
	showItems(secondItems);

	printf("Inserting 0 as 23rd element of third list:\n");
	insertDLL(thirdItems,22,newINTEGER(0));
	showItems(thirdItems);

	printf("Removing everything from both lists.\n");

	INTEGER *n;
	INTEGER *m;

	for (count = 0; count < 65; ++count) {
		n = removeDLL(secondItems,0);
	
		freeINTEGER(n);

		m = removeDLL(thirdItems,sizeDLL(thirdItems) - 1);

		freeINTEGER(m);
	}

	printf("The second list is now:\n");
	showItems(secondItems);

	printf("The third list is now:\n");
	showItems(thirdItems);

	printf("Inserting 1000 elements into each list:\n");

	for (count = 0; count < 1000; ++count) {
		insertDLL(secondItems,0,newINTEGER(count));
		insertDLL(thirdItems,0,newINTEGER(5 * count));
	}

	printf("The second list is now:\n");
	showItems(secondItems);

	printf("The third list is now:\n");
	showItems(thirdItems);

	printf("Testing union of second and third lists:\n");

	unionDLL(secondItems,thirdItems);

	printf("The second list is now:\n");
	showItems(secondItems);

	printf("The third list is now:\n");
	showItems(thirdItems);

	printf("Testing firstDLL and currentDLL\n");
	firstDLL(secondItems);
	printf("List head is: ");
	displayINTEGER((INTEGER *) currentDLL(secondItems), stdout);
	printf("\n");
	printf("Testing nextDLL and moreDLL\n");
	nextDLL(secondItems);
	printf("Second node is: ");
	displayINTEGER((INTEGER *) currentDLL(secondItems), stdout);
	printf("\n");
	printf("Second node isn't NULL: ");
	(moreDLL(secondItems) == 1) ? (printf("True\n")) : (printf("False\n"));
	printf("Testing prevDLL and moreDLL(backwards twice)\n");
	prevDLL(secondItems);
	printf("Current node is now: ");
	displayINTEGER((INTEGER *) currentDLL(secondItems), stdout);
	printf("\n");
	prevDLL(secondItems);
	printf("Second node isn't NULL: ");
	(moreDLL(secondItems) == 1) ? (printf("True\n")) : (printf("False\n"));
	printf("Inserting new node at tail (-1)\n");
	// value is really a DLLNODE
	void *value = insertDLL(secondItems, sizeDLL(secondItems), newINTEGER(-1));
	showItems(secondItems);

	printf("Testing removeDLLnode:\n");
	printf("Removed node was ");
	INTEGER *u = removeDLLnode(secondItems, value);
	displayINTEGER((INTEGER *) u, stdout);
	printf(".\n");
	freeINTEGER(u);

//	printf("Testing removeDLLall:\n");
//	removeDLLall(secondItems);
	
	printf("Freeing the second list.\n");
	freeDLL(secondItems);

	printf("Freeing the third list.\n");
	freeDLL(thirdItems);

	return 0;
}

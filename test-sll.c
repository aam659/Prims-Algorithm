#include <stdio.h>
#include <stdlib.h>
#include "sll.h"
#include "integer.h"
#include "real.h"
#include "string.h"

static void showItems(SLL *items) {
	printf("The items are ");
       	displaySLL(items,stdout);
       	printf(".\n");
       	printf("The items (debugged) are ");
       	displaySLLdebug(items,stdout);
       	printf(".\n");
}

int main(int argc,char **argv) {
	int count = 0;

	if (argc != 1) {
        	fprintf(stderr,"usage: %s\n",argv[0]);
        	exit(1);
        }

       	SLL *items = newSLL(displayINTEGER,freeINTEGER);
	SLL *secondItems = newSLL(displayINTEGER,freeINTEGER);
	SLL *thirdItems = newSLL(displayINTEGER,freeINTEGER);

       	showItems(items);
       	insertSLL(items,0,newINTEGER(3));                   	//insert at front
       	insertSLL(items,sizeSLL(items),newINTEGER(2));      	//insert at back
       	insertSLL(items,1,newINTEGER(1));                   	//insert at middle
       	showItems(items);
       	printf("The value ");
       	INTEGER *i = removeSLL(items,0);                   	//remove from front
       	displayINTEGER(i,stdout);
       	printf(" was removed.\n");
       	freeINTEGER(i);
       	showItems(items);
       	int x = getINTEGER((INTEGER *) getSLL(items,0));    	//get the first item
       	printf("The first item is %d.\n",x);
//	INTEGER *j = newINTEGER(7);
//	setSLL(items, 0, j);
//	INTEGER *k = removeSLL(items,0);
//	printf("The first item is now ");
//	displayINTEGER(k,stdout);
//	printf("\n");
//	freeINTEGER(k);
	showItems(items);
	printf("The size of the list is %d\n", sizeSLL(items));
       	printf("Freeing the list.\n");
       	freeSLL(items);

	printf("Printing the second list:\n");
	showItems(secondItems);
	printf("Printing the third list:\n");
	showItems(thirdItems);

	for (count = 0; count < 64; ++count) {
		insertSLL(secondItems, 0, newINTEGER(count));
		insertSLL(thirdItems, 0, newINTEGER(2 * count));
	}

	printf("Second list after 64 inserts:\n");
	showItems(secondItems);

	printf("Third list after 64 inserts:\n");
	showItems(thirdItems);

	printf("Inserting 0 into 32nd element of second list:\n");
	insertSLL(secondItems,(sizeSLL(secondItems) / 2),newINTEGER(0));
	showItems(secondItems);
	
	printf("Inserting 0 as 23rd element of third list:\n");
	insertSLL(thirdItems,22,newINTEGER(0));
	showItems(thirdItems);

	printf("Removing everything from both lists.\n");

	INTEGER *n;
	INTEGER *m;
	
	for (count = 0; count < 65; ++count) {
		n = removeSLL(secondItems,0);

		freeINTEGER(n);		

		m = removeSLL(thirdItems,sizeSLL(thirdItems) - 1);

		freeINTEGER(m);
	}

	printf("The second list is now:\n");
	showItems(secondItems);

	printf("The third list is now:\n");
	showItems(thirdItems);

	printf("Inserting 1000 elements into each list:\n");
	
	for (count = 0; count < 1000; ++count) {
		insertSLL(secondItems, 0, newINTEGER(count));
		insertSLL(thirdItems, 0, newINTEGER(5 * count));
	}

	printf("The second list is now:\n");
	showItems(secondItems);

	printf("The third list is now:\n");
	showItems(thirdItems);

	printf("Testing union of second and third lists:\n");
	
	unionSLL(secondItems, thirdItems);

	printf("The second list is now:\n");
	showItems(secondItems);

	printf("The third list is now:\n");
	showItems(thirdItems);

	printf("Freeing the second list.\n");
       	freeSLL(secondItems);
	
	printf("Freeing the third list.\n");
	freeSLL(thirdItems);
       	
	return 0;
}

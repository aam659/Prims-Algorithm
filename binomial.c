/* Author: Alan Manning */
/* Class: CS201 Data Structures and Algorithms */
/* Assignment 3 */
/* Date: 3 March, 2018 */

/* This is the implementation file for the header file 
"binomial.h", which is used to create a binomial heap 'class'
for generic data types. */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <stdbool.h>
#include "binomial.h"
//#include "avl.h"

// Binomial Tree Definiton
struct binomial {
	void (*disp)(void *,FILE *);				// display method
	int (*compare)(void *,void *);				// compare method
	void (*update)(void *,void *);				// update method
	void (*free)(void *);					// free method
	DLL *rootList;						// rootlist
	void *min;						// extreme pointer (DLLNODE)
	//AVL *vertexTree;					// vertex tree
	//AVL *edgeTree;					// edge tree
	int size;						// size field
};

/*************** Private Prototypes ***************/

/*************** Public Definitons ***************/

/* Binomial Heap Constructor */
/* This is the constructor for the Binomial heap objects. */

BINOMIAL *newBINOMIAL(
	void (*display)(void *,FILE *),
	int (*comparator)(void *,void *),
	void (*updater)(void *,void *),
	void (*freer)(void *)) {
	BINOMIAL *b = malloc(sizeof(BINOMIAL));			// Allocate space

	assert(b != 0);
	b->size = 0;
	b->disp = display;
	b->compare = comparator;
	b->free = freer;

	b->rootList = newDLL(b->disp, b->free);			// Initialize rootlist
	b->min = 0;

	//TODO: INITIALIZE AVL TREES????	


	return b;
}

/* insertBINOMIAL Method */
/* Inserts into heap. Returns address of private node
that holds inserted value. */

void *insertBINOMIAL(BINOMIAL *b, void *value) {



}

/* sizeBINOMIAL Method */
/* Accessor for size of Binomial heap. */

int sizeBINOMIAL(BINOMIAL *b) { return b->size; }

/* unionBINOMIAL Method */
/* Unions 2 Binomial heaps. */
void unionBINOMIAL(BINOMIAL *b1, BINOMIAL *b2) {


	return;
}

/* deleteBINOMIAL Method */
/* Removes value in passed node from heap. */

void deleteBINOMIAL(BINOMIAL *b, void *node) {

	return;
}

/* decreaseKeyBINOMIAL Method */
/* Decreases key for value in passed node to 3rd argument. */
void decreaseKeyBINOMIAL(BINOMIAL *b, void *node, void *value) {

	return;
}

/* peekBINOMIAL Method */
/* Returns min value in heap. */

void *peekBINOMIAL(BINOMIAL *b) {


}

/* extractBINOMIAL Method */
/* Removes and returns min value in heap. */

void *extractBINOMIAL(BINOMIAL *b) {


}

/* statisticsBINOMIAL Method */
/* Displays number of values in heap and rootlist size. */

void statisticsBINOMIAL(BINOMIAL *b, FILE *fp) {


	return;
}

/* displayBINOMIAL Method */
/* Displays rootlist values for Binomial heap. */

void displayBINOMIAL(BINOMIAL *b, FILE *fp) {


}

/* displayBINOMIALdebug Method */
/* Displays entire heap using level-order traversal of subheaps. */

void displayBINOMIALdebug(BINOMIAL *b,FILE *fp) {

	return;
}

/* freeBINOMIAL Method */
/* Frees Binomial heap. */

void freeBINOMIAL(BINOMIAL *b) {


	free(b);

	return;
}

/*************** Private Definitons ***************/

/* bubbleUp Method */
/* Bubbles a binomial heap node up a binomial tree. */



//TODO: IMPLEMENT CODE BELOW
/* 
static void bubbleUp(n) {
	var p = n->parent;
        if (n == p) return n;
        if (compare(n->value,p->value) >= 0) return n;
        if (update function exists) update(n->value,p);
        if (update function exists) update(p->value,n);
        var temp = n->value;
        n->value = p->value;
        p->value = temp;
        return bubbleUp(b,p);
}*/

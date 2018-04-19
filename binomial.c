/* Author: Alan Manning */
/* Class: CS201 Data Structures and Algorithms */
/* Assignment 3 */
/* Date: 3 April, 2018 */

/* This is the implementation file for the header file 
"binomial.h", which is used to create a binomial heap 'class'
for generic data types. */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>
#include "binomial.h"
#include "dll.h"
#include "queue.h"

typedef struct BINNODE BIN;

struct BINNODE {
	void *value;						// BINNODE value
	void (*disp)(void *,FILE *);				// display method
	void (*free)(void *);					// free method
	BIN *parent;						// BIN parent pointer
	void *owner;						// DLLNODE owner pointer
	DLL *children;						// DLL children pointer
};

// Binomial Heap Definiton
struct binomial {
	void (*disp)(void *,FILE *);				// display method
	int (*compare)(void *,void *);				// compare method
	void (*update)(void *,void *);				// update method
	void (*free)(void *);					// free method
	DLL *rootList;						// rootlist
	BIN *min;						// extreme pointer (BIN)
	int size;						// size field
};

/*************** Private Prototypes ***************/
static BIN *newBIN(
	void (*)(void *,FILE *),
	void (*)(void *),
	void *);
static DLL *getRootList(BINOMIAL *);
static BIN *getMin(BINOMIAL *);
static void setMin(BINOMIAL *,BIN *);
static void *getBINvalue(BIN *);
static void setBINvalue(BIN *,void *);
static BIN *getBINparent(BIN *);
static void setBINparent(BIN *,BIN *);
static DLL *getBINchildren(BIN *);
static void *getBINowner(BIN *);
static void setBINowner(BIN *,void *);
static void setBINOMIALsize(BINOMIAL *,int);
static BIN *bubbleUp(BINOMIAL *,BIN *);
static void consolidate(BINOMIAL *);
static void updateConsolidationArray(BINOMIAL *,BIN **,void *);
static BIN *combine(BINOMIAL *,BIN *,BIN *);
static void displayBIN(void *, FILE *fp);
static void freeBIN(void *);

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
	b->update = updater;
	b->free = freer;

	b->rootList = newDLL(displayBIN, freeBIN);		// Initialize rootlist
	b->min = 0;

	return b;
}

/* insertBINOMIAL Method */
/* Inserts into heap. Returns address of private node
that holds inserted value. */

void *insertBINOMIAL(BINOMIAL *b, void *value) {
	BIN *n = newBIN(b->disp, b->free, value);		// Allocate space
	int binomialSize = sizeBINOMIAL(b);			// Binomial heap size
	DLL *rootList = 0;
	
	rootList = getRootList(b);

	insertDLL(rootList, 0, n);				// Insert n into rootList

	setBINOMIALsize(b, binomialSize + 1);			// Update binomial heap size

	consolidate(b);

	return n;
}

/* sizeBINOMIAL Method */
/* Accessor for size of Binomial heap. */

int sizeBINOMIAL(BINOMIAL *b) { return b->size; }

/* unionBINOMIAL Method */
/* Unions 2 Binomial heaps. */

void unionBINOMIAL(BINOMIAL *b1, BINOMIAL *b2) {
	DLL *rootList1 = getRootList(b1);
	DLL *rootList2 = getRootList(b2);
	int recipientSize = 0;

	unionDLL(rootList1, rootList2);				// Merge rootlists

	recipientSize = sizeDLL(rootList1);

	setBINOMIALsize(b1, recipientSize);			// Update recipient size

	setBINOMIALsize(b2, 0);					// Update donor size

	setMin(b2, 0);						// Set extreme value 0

	consolidate(b1);

	return;
}

/* deleteBINOMIAL Method */
/* Removes value in passed node from heap. */

void deleteBINOMIAL(BINOMIAL *b, void *node) {
	decreaseKeyBINOMIAL(b, node, 0);			// Call decrease key to 0
	extractBINOMIAL(b);					// Extract min

	return;
}

/* decreaseKeyBINOMIAL Method */
/* Decreases key for value in passed node to 3rd argument. */

void decreaseKeyBINOMIAL(BINOMIAL *b, void *node, void *value) {
	BIN *bN = node;
	BIN *min = getMin(b);
	void *minValue = getBINvalue(min);
	BIN *n = 0;
	void *nodeValue = 0;

	setBINvalue(bN, value);					// Update BINNODE value

	n = bubbleUp(b, bN);
	nodeValue = getBINvalue(n);	

	if (b->compare(minValue, nodeValue) > 0) {
		setMin(b, n);					// Update min value
	}

	return;
}

/* peekBINOMIAL Method */
/* Returns min value in heap. */

void *peekBINOMIAL(BINOMIAL *b) {
	BIN *min = getMin(b);					// Get min node
	void *minValue = getBINvalue(min);			// Get min value

	return minValue;
}

/* extractBINOMIAL Method */
/* Removes and returns min value in heap. */

void *extractBINOMIAL(BINOMIAL *b) {
	BIN *min = getMin(b);					// Get min node
	BIN *removedValue = 0;
	void *value = getBINvalue(min);				// Get min value
	int binSize = sizeBINOMIAL(b);
	void *owner = getBINowner(min);				// Access DLL owner node
	DLL *rootList = getRootList(b);				// Access rootList
	DLL *children = getBINchildren(min);
	firstDLL(children);					// Iterator for head of DLL
	
	removedValue = removeDLLnode(rootList, owner);		// Remove from rootList
	BIN *current = 0;

	while (moreDLL(children)) {
		current = currentDLL(children);
		// Set each child's parent to point to itself
		setBINparent(current, current);
		nextDLL(children);				// Update iterator
	}
	
	unionDLL(children, rootList);
	unionDLL(rootList, children);				// Merge children DLL to rootList

	consolidate(b);						// Consolidate b's root list
	
	setBINOMIALsize(b, binSize - 1);			// Decrement binomial heap size

	freeDLL(getBINchildren(removedValue));
	free(removedValue);	

	return value;
}

/* statisticsBINOMIAL Method */
/* Displays number of values in heap and rootlist size. */

void statisticsBINOMIAL(BINOMIAL *b, FILE *fp) {
	int binomialSize = sizeBINOMIAL(b);
	DLL *rootList = getRootList(b);
	int rootListSize = sizeDLL(rootList);
	BIN *min = getMin(b);
	void *value = 0;

	fprintf(fp, "size: %d\n", binomialSize);
	fprintf(fp, "rootlist size: %d\n", rootListSize);
	
	if (min == 0) return;

	value = getBINvalue(min);

	fprintf(fp, "extreme: ");

	min->disp(value, fp);

	fprintf(fp, "\n");	

	return;
}

/* displayBINOMIAL Method */
/* Displays rootlist values for Binomial heap. */

void displayBINOMIAL(BINOMIAL *b, FILE *fp) {
	int degree = 0;
	int rootDegree = 0;
	DLL *rootList = 0;
	BIN *n = 0;
	DLL *nChildren = 0;
	BIN *min = 0;
	void *value = 0;
	void *minValue = 0;

	if (b == 0) {						// Empty Binomial heap
		fprintf(fp, "NULL\n");
		return;
	}

	rootList = getRootList(b); 
	firstDLL(rootList);					// Point to rootList head
	n = currentDLL(rootList);				// BIN node at head
	nChildren = getBINchildren(n);				// Retrieve node children
	rootDegree = sizeDLL(nChildren);			// Size of children DLL
	min = getMin(b);

	if (min != 0) { minValue = getBINvalue(min); }		// Get min value

	fprintf(stdout, "rootlist: ");

	while (moreDLL(rootList)) {
		if (degree < rootDegree) {
			fprintf(fp, "NULL");
		}

		else if (degree == rootDegree) {
			value = getBINvalue(n);
			b->disp(value, fp);
			if (value == minValue) { fprintf(fp, "*"); }
			nextDLL(rootList);
		}
			
		++degree;

		if (moreDLL(rootList)) { 
			n = currentDLL(rootList); 
			nChildren = getBINchildren(n);
			rootDegree = sizeDLL(nChildren);
			fprintf(fp, " ");
		}
	}

	fprintf(fp, "\n");

	return;
}

/* displayBINOMIALdebug Method */
/* Displays entire heap using level-order traversal of subheaps. */

void displayBINOMIALdebug(BINOMIAL *b,FILE *fp) {
	DLL *currentList = getRootList(b);
	DLL *children = 0;
	QUEUE *parentQueue = newQUEUE(displayBIN, freeBIN);
	QUEUE *childQueue = newQUEUE(displayBIN, freeBIN);
	BIN *n = 0;
	
	if (sizeDLL(currentList) == 0) { 
		displayDLL(currentList, fp);			// Empty rootlist
		fprintf(fp, "\n");
		return;
	}

	enqueue(parentQueue, currentList);			// Add DLL to queue
	
	while (sizeQUEUE(parentQueue) != 0) {
		currentList = peekQUEUE(parentQueue);
		firstDLL(currentList);				// Initialize Iterator

		displayDLL(currentList, fp);

		while (moreDLL(currentList)) {
			n = currentDLL(currentList);		// Grab node from DLL
		
			children = getBINchildren(n);		// Get BIN node children
		
			if (sizeDLL(children) != 0) {
				enqueue(childQueue, children);
			}

			nextDLL(currentList);			// update iterator
		}

		dequeue(parentQueue);

		if (sizeQUEUE(parentQueue) == 0) {
			
			if (sizeQUEUE(parentQueue) == 0) {
				fprintf(fp, "\n");
			}

			while (sizeQUEUE(childQueue) > 0) {
				enqueue(parentQueue, (DLL *) dequeue(childQueue));
			}

		}
	}

	freeQUEUE(parentQueue);
	freeQUEUE(childQueue);

	return;
}

/* freeBINOMIAL Method */
/* Frees Binomial heap. */

void freeBINOMIAL(BINOMIAL *b) {
	DLL *rootList = getRootList(b);

	freeDLL(rootList);

	free(b);

	return;
}

/*************** Private Definitons ***************/

/* BINNODE Node Constructor */

static BIN *newBIN(
	void (*d)(void *,FILE *),
	void (*f)(void *),
	void *value) {
	BIN *binNode = malloc(sizeof(BIN));

	assert(binNode != NULL);

	binNode->value = value;
	binNode->parent = binNode;					// Points to BIN that owns it
	binNode->owner = 0;					// Points to DLLNODE that owns it
	binNode->children = newDLL(displayBIN, freeBIN);
	binNode->disp = d;
	binNode->free = f;

	return binNode;
}

/* getBINowner Method */
/* Returns owner for binomial tree node. */

static void *getBINowner(BIN *bN) { return bN->owner; }

/* setBINowner Method */
/* Sets owner for binomial tree node. */

static void setBINowner(BIN *bN, void *o) {
	bN->owner = o;

	return;
}

/* getBINparent Method */
/* Returns parent for binomial tree node. */

static BIN *getBINparent(BIN *bN) { return bN->parent; }

/* setBINparent Method */
/* Sets parent for binomial tree node. */

static void setBINparent(BIN *bN, BIN *p) {
	bN->parent = p;

	return;
}

/* getBINchildren Method */
/* Returns children DLL for binomial tree node. */

static DLL *getBINchildren(BIN *bN) { return bN->children; }

/* setBINchildren Method */
/* Sets children for binomial tree node. */
/*
static void setBINchildren(BIN *bN, DLL *c) {
	bN->children = c;

	return;
}*/

/* getRootList Method */
/* Returns root list of Binomial heap. */

static DLL *getRootList(BINOMIAL *b) { return b->rootList; }

/* setRootList Method */
/* Sets root list of Binomial heap. */
/*
static void setRootList(BINOMIAL *b, DLL *d) {
	if (d == 0) return;

	b->rootList = d;

	return;	
}*/

/* getMin Method */
/* Returns extreme value of Binomial heap. */

static BIN *getMin(BINOMIAL *b) { return b->min; }

/* setMin Method */
/* Sets extreme value of Binomial heap. */

static void setMin(BINOMIAL *b, BIN *m) {
	b->min = m;

	return;
}

/* getBINvalue Method */
/* Returns value for binomial tree node. */

static void *getBINvalue(BIN *bN) { return bN->value; }

/* setBINvalue Method */
/* Sets value for binomial tree node. */

static void setBINvalue(BIN *bN, void *v) {
	bN->value = v;

	return;
}

static void setBINOMIALsize(BINOMIAL *b, int s) {
	b->size = s;

	return;
}

/* bubbleUp Method */
/* Bubbles a binomial heap node up a binomial tree. */

static BIN *bubbleUp(BINOMIAL *b, BIN *n) {
	BIN *p = 0;
	void *nValue = 0;
	void *pValue = 0;
	void *temp = 0;
	
	if (n != 0) { p = getBINparent(n); }
	if (n != 0) { nValue = getBINvalue(n); }		// Set values
	if (p != 0) { pValue = getBINvalue(p); }

	// n is its own parent, so root of a subheap
        if (n == p) return n;					// Comparisons
        if (b->compare(nValue,pValue) >= 0) return n;
        if (b->update != 0) b->update(nValue,p);		// Update owners
        if (b->update != 0) b->update(pValue,n);		// Update owners
       
	temp = nValue;						// Swap values
        setBINvalue(n, pValue);
	setBINvalue(p, temp);
        
	return bubbleUp(b,p);
}

/* consolidate Method */
/* Moves all subheaps from rootList to consolidation
array, then moves these back to rootList. */

static void consolidate(BINOMIAL *b) {
	if (b == 0) return;
	int i = 0;
	int binomialSize = sizeBINOMIAL(b);
	// Consolidation array size
	//int arraySize = (int)(log((double)binomialSize)/log(2) + 1.00);
	int arraySize = 0;
	arraySize = (int)(log(binomialSize)/log(2)) + 1;
	DLL *rootList = 0;
	int rootListSize = 0;
	BIN *spot = 0;
	BIN *node = 0;
	BIN *min = 0;
	void *dllNode = 0;

	// Creates consolidation array D
	BIN **D = malloc(sizeof(BIN *) * arraySize);		// Allocate array

	while (i < arraySize) { D[i++] = 0; }			// Initialize to 0

	i = 0;

	rootList = getRootList(b);

	rootListSize = sizeDLL(rootList);
	firstDLL(rootList);					// Get DLL head	

	// Place all the subheaps in the D array, combining as needed
	while (rootListSize > 0) {
		spot = removeDLL(rootList, 0);			// Remove spot
		
		updateConsolidationArray(b, D, spot);		// Update D array
		rootList = getRootList(b);
		rootListSize = sizeDLL(rootList);
	}

	// Transfer D array back to heap, keeping track of extreme value
	setMin(b, 0);						// Set extreme to 0
	min = getMin(b);

	while (i < arraySize) {
		rootList = getRootList(b);
		if (D[i] != 0) {
			// Insert D[i] into rootList
			dllNode = insertDLL(rootList, sizeDLL(rootList), D[i]);
			node = D[i];
			setBINowner(node, dllNode);
	
			// Update extreme value, if needed
			if (min == 0) { setMin(b, D[i]); }
			
			else if (b->compare(getBINvalue(min), getBINvalue(node)) > 0) {
				setMin(b, D[i]);
			}
		}

		++i;
		min = getMin(b);
	}	

	free(D);		

	return;
}


/* updateConsolidationArray Method */
/* Updates consolidation array. */

static void updateConsolidationArray(BINOMIAL *b, BIN **D, void *spot) {
	if (spot == 0) return;
	BIN *bN = spot;
	DLL *children = getBINchildren(bN);
	int degree = sizeDLL(children);

	while (D[degree] != 0) {
		bN = combine(b, bN, D[degree]);			// Combines spot and D[degree]
								// Spot is new combined subheap
		D[degree++] = 0;				// Slot is now empty; increment
	}

	D[degree] = bN;						// Set D[degree] to spot	

	return;
}


/* combine Method */
/* Combines two subheaps and makes the subheap whose root
is less extreme a child of the other. */

static BIN *combine(BINOMIAL *b, BIN *x, BIN *y) {
	DLL *xChildren = getBINchildren(x);
	DLL *yChildren = getBINchildren(y);
	void *xValue = 0;
	void *yValue = 0;
	if (x != 0) { xValue = getBINvalue(x); }
	if (y != 0) { yValue = getBINvalue(y); }

	if (b->compare(xValue, yValue) < 0) {
		// Insert y into the children of x
		setBINowner(y, insertDLL(xChildren, sizeDLL(xChildren), y));
		// Set the parent of y to x
		setBINparent(y, x);

		return x;
	}

	// Insert x into the children of y
	setBINowner(x, insertDLL(yChildren, sizeDLL(yChildren), x));
	// Set the parent of x to y
	setBINparent(x, y);

	return y;
}

/* displayBIN Method */
/* Custom display method to display BIN values. */

static void displayBIN(void *v, FILE *fp) {
	BIN *bN = v;
	void *value = getBINvalue(bN);
	bN->disp(value, fp);

	return;
}

/* freeBIN Method */
/* Custom freeing method to free BIN values and nodes. */

static void freeBIN(void *v) {
	BIN *oldBen = v;					// Insert Star Wars joke here
	DLL *children = getBINchildren(v);
	void *old = getBINvalue(oldBen);

	if (oldBen->free != NULL) { oldBen->free(old); }	// Free node value

	freeDLL(children);
	free(oldBen);

	return;
}

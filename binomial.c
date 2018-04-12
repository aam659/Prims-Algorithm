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
#include <math.h>
#include "binomial.h"
#include "dll.h"

typedef struct BINNODE BIN;

struct BINNODE {
	void *value;						// BINNODE value
	void (*disp)(void *,FILE *);				// display method
	void (*free)(void *);					// free method
	BIN *parent;						// BIN parent pointer
	void *owner;						// DLLNODE owner pointer
	DLL *children;						// DLL children pointer
};

// Binomial Tree Definiton
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
static void setRootList(BINOMIAL *,DLL *);
static BIN *getMin(BINOMIAL *);
static void setMin(BINOMIAL *,BIN *);
static void *getBINvalue(BIN *);
static void setBINvalue(BIN *,void *);
static BIN *getBINparent(BIN *);
static void setBINparent(BIN *,BIN *);
static DLL *getBINchildren(BIN *);
static void setBINchildren(BIN *,DLL *);
static void *getBINowner(BIN *);
static void setBINowner(BIN *,void *);
static void setBINOMIALsize(BINOMIAL *,int);
static BIN *bubbleUp(BINOMIAL *,BIN *);
static void consolidate(BINOMIAL *);
static void updateConsolidatedArray(void **,void *);
static BINOMIAL *combine(BINOMIAL *,BINOMIAL *,BINOMIAL *);

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

	b->rootList = newDLL(b->disp, b->free);			// Initialize rootlist
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
	DLL *children = newDLL(n->disp, n->free);
	
	setBINparent(n, n);					// Set n to own parent

	setBINchildren(n, children);				// Initialize children DLL

	rootList = getRootList(b);

	insertDLL(rootList, 0, n);				// Insert n into rootlist

	setBINOMIALsize(b, binomialSize + 1);			// Update binomial heap size

	// TODO: CONSOLIDATE ROOT LIST OF B USING B'S COMPARATOR

	return n;
}

/* sizeBINOMIAL Method */
/* Accessor for size of Binomial heap. */

int sizeBINOMIAL(BINOMIAL *b) { return b->size; }

/* unionBINOMIAL Method */
/* Unions 2 Binomial heaps. */
/*
void unionBINOMIAL(BINOMIAL *b1, BINOMIAL *b2) {
	DLL *rootList1 = getRootList(b1);
	DLL *rootList2 = getRootList(b2);
	int recipientSize = 0;

	unionDLL(rootList1, rootList2);				// Merge rootlists

	recipientSize = sizeDLL(rootList1);

	setBINOMIALsize(b1, rootList1);				// Update recipient size

	// TODO: NEED TO UPDATE DONOR ROOTLIST TO NULL????

	setBINOMIALsize(b2, 0);					// Update donor size

	setMin(b2, 0);						// Set extreme value 0

	// TODO: CALL CONSOLIDATE METHOD FOR ROOT LIST OF B USING B'S COMPARATOR	

	return;
}*/

/* deleteBINOMIAL Method */
/* Removes value in passed node from heap. */

void deleteBINOMIAL(BINOMIAL *b, void *node) {
//	decreaseKeyBINOMIAL(b, node, 0);			// Call decrease key to 0
//	extractBINOMIAL(b);					// Extract min

	return;
}

/* decreaseKeyBINOMIAL Method */
/* Decreases key for value in passed node to 3rd argument. */

void decreaseKeyBINOMIAL(BINOMIAL *b, void *node, void *value) {
//	BIN *bN = node;

//	setBINvalue(bN, value);					// Update BINNODE value

//	bubbleUp(b, bN); 	

	// TODO: RETURN NODE WHERE NEW VALUE FINISHES UP???
	// TODO: UPDATE B'S EXTREME VALUE POINTER, IF NECESSARY???

	return;
}

/* peekBINOMIAL Method */
/* Returns min value in heap. */
/*
void *peekBINOMIAL(BINOMIAL *b) {


}*/

/* extractBINOMIAL Method */
/* Removes and returns min value in heap. */
/*
void *extractBINOMIAL(BINOMIAL *b) {


}*/

/* statisticsBINOMIAL Method */
/* Displays number of values in heap and rootlist size. */
/*
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
}*/

/* displayBINOMIAL Method */
/* Displays rootlist values for Binomial heap. */
/*
void displayBINOMIAL(BINOMIAL *b, FILE *fp) {
	int degree = 0;
	int rootDegree = 0;
	int rootListSize = 0;
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
	rootListSize = sizeDLL(rootList);
	n = firstDLL(rootList);					// Point to rootList head
	nChildren = getBINchildren(n);				// Retrieve node children
	rootDegree = sizeDLL(nChildren);			// Size of children DLL
	min = getMin(b);
	if (min != 0) { minValue = getBINvalue(min); }		// Get min value

	while (degree < rootListSize) {
		if (degree < rootDegree) {
			fprintf(fp, "NULL");
		}

		else if (degree == rootDegree) {
			value = getBINvalue(n);
			b->disp(value, fp);
			if (value == minValue) { fprintf(fp, "*"); }
		}
			
		++degree;
		if (degree == rootListSize) { fprintf(fp, "\n"); }
		else { fprintf(fp, " "); }
		n = moreDLL(rootList);
		nChildren = getBINchildren(n);
		rootDegree = sizeDLL(nChildren);
	}

	return;
}*/

/* displayBINOMIALdebug Method */
/* Displays entire heap using level-order traversal of subheaps. */

/*
void displayBINOMIALdebug(BINOMIAL *b,FILE *fp) {
	//FIXME: CORRECT LEVEL-ORDER TRAVERSAL AND DLL PRINTING

	DLL *rootList = getRootList(b);
	int childListSize = 0;
	
	if (sizeDLL(rootList == 0)) { 
		displayDLL(rootList, fp);			// Empty rootlist
		fprintf(fp, "\n");
		return;
	}

	BIN *n = firstDLL(rootList);				// Head of list
	DLL *childrenList = getBINchildren(n);			// Children list
	childListSize = sizeDLL(childrenList);
	DLL *traversalList = rootList;				// Traversal list

	displayDLL(rootList, fp);
	fprintf(fp, "\n");
	
	while (childListSize > 0) {
		displayDLL(traversalList, fp);			// Display rootlist
		fprintf(fp, "\n");
		
		
	}
	
	return;
}*/

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
	binNode->parent = 0;					// Points to BIN that owns it
	binNode->owner = 0;					// Points to DLLNODE that owns it
	binNode->children = 0;
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
	if (o == 0) return;

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
static void setBINchildren(BIN *bN, DLL *c) {
	bN->children = c;

	return;
}

/* getRootList Method */
/* Returns root list of Binomial heap. */

static DLL *getRootList(BINOMIAL *b) { return b->rootList; }

/* setRootList Method */
/* Sets root list of Binomial heap. */

static void setRootList(BINOMIAL *b, DLL *d) {
	if (d == 0) return;

	b->rootList = d;

	return;	
}

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
        if (b->compare(n->value,p->value) >= 0) return n;
        if (b->update != 0) b->update(n->value,p);		// Update owners
        if (b->update != 0) b->update(p->value,n);		// Update owners
       
	temp = nValue;						// Swap values
        setBINvalue(n, p->value);
	setBINvalue(p, temp);
        
	return bubbleUp(b,p);
}

/* consolidate Method */
/* Moves all subheaps from rootList to consolidation
array, then moves these back to rootList. */

static void consolidate(BINOMIAL *b) {
	if (b == 0) return;
	int i = 0;
	int j = 0;
	int binomialSize = sizeBINOMIAL(b);
	int arraySize = (int)(log((double)binomialSize)/log(2) + 1.00);
	DLL *rootList = 0;
	int rootListSize = 0;
	void *spot = 0;
	void *temp = 0;
	BIN *min = 0;

	void **D = malloc(sizeof(void *) * arraySize);		// Allocate array

	while (i < arraySize) { D[i++] = 0 };			// Initialize to 0

	i = 0;

	rootList = getRootList(b);

	rootListSize = sizeDLL(rootList);

	// Place all the subheaps in the D array, combining as needed
	while (rootListSize > 0) {
		// TODO: USE ITERATOR?????
		spot = firstDLL(rootList);			// Get DLL head	
		temp = removeDLLnode(rootList, spot);		// Remove spot
		
		//FIXME: CORRECT LINE BELOW
		D[j++] = temp;					// Update D array	
		rootList = getRootList(b);
		rootListSize = sizeDLL(rootList);
	}

	// Transfer D array back to heap, keeping track of extreme value
	setMin(b, 0);						// Set extreme to 0
	min = getMin(b);

	while (i < arraySize) {
		if (D[i] != 0) {
			insertDLL(rootList, 0, D[i]);
	
			// Update extreme value, if needed
			if (min == 0) { setMin(b, D[i]); }
			
			else if (b->compare(min, D[i] > 0)) {
				setMin(b, D[i]);
			}
		}

		++i;
	}	

	//TODO: FREE THE D ARRAY (IF NEEDED)

	return;
}


static void updateConsolidationArray(void **D, void *spot) {
	BIN *bN = spot;
	DLL *children = getBINchildren(bN);
	int degree = sizeDLL(children);

	while (D[degree] != 0) {
//		combine(b

	}	

	return;
}


//TODO: SHOULDN'T X AND Y BE BINNODES???????

static BINOMIAL *combine(BINOMIAL *b, BINOMIAL *x, BINNOMIAL *y) {
	BIN *xMin = getMin(x);
	BIN *yMin = getMin(y);
	DLL *xChildren = getBINchildren(x);
	DLL *yChildren = getBINchildren(y);

	if (b->compare(yMin, xMin)) {
		// Insert y into the children of x
		insertDLL(xChildren, 0, y);
		// Set the parent of y to x
		setBINparent(y, x);

		return x;
	}

	// Insert x into the children of y
	insertDLL(yChildren, 0, x);
	// Set the parent of x to y
	setBINparent(x, y);

	return y;
}

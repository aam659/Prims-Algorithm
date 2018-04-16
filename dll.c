/* Author: Alan Manning */
/* Class: CS201 Data Structures and Algorithms */
/* Assignment 0 */
/* Date: 25 January, 2018 */

/* This is the implementation file for the header file
'dll.h', which is used to create a stack 'class' for
generic data types.*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <string.h>
#include "dll.h"

/* dllNODE Definition */
typedef struct dllNODE DLLNODE;

struct dllNODE {
	void *value;						// DLL data
	DLLNODE *next;						// next pointer
	DLLNODE *previous;					// previous pointer
};

struct dll {
	int size;						// DLL size
	DLLNODE *head;						// head node
	DLLNODE *tail;						// tail node
	void (*display)(void *, FILE*);				// display field
	void (*free)(void *);					// free field
	DLLNODE *iterator;					// private iterator
};

/*************** private prototypes ***************/
static DLLNODE *newNODE(void *);
static DLLNODE *search(int,DLLNODE *);
static DLLNODE *searchReverse(int,int,DLLNODE *);
static void insertLast(DLL *,DLLNODE *,DLLNODE *);
static void insertFirst(DLL *,DLLNODE *,DLLNODE *);
static void insertMiddle(DLLNODE *,DLLNODE *);
static void setHead(DLL *,DLLNODE *);
static DLLNODE *getHead(DLL *);
static void setTail(DLL *,DLLNODE *);
static DLLNODE *getTail(DLL *);
//static void setDLLNODEvalue(DLLNODE *,void *);
static void *getDLLNODEvalue(DLLNODE *);
static void setDLLNODEnext(DLLNODE *,DLLNODE *);
static DLLNODE *getDLLNODEnext(DLLNODE *);
static void setDLLNODEprevious(DLLNODE *,DLLNODE *);
static DLLNODE *getDLLNODEprevious(DLLNODE *);
//static void setDLLNODE(DLLNODE *,DLLNODE *);
//static void *getDLLNODE(DLLNODE *);
static void setIterator(DLL *,DLLNODE *);
static void *getIterator(DLL *);
static void setDLLsize(DLL *,int);

/*************** public implementations ***************/

/* constructor */
/* This is the constructor for the DLL objects. */

DLL *newDLL(void (*d)(void *,FILE *),void (*f)(void *)) {
	DLL *items = malloc(sizeof(DLL));
	assert(items != 0);
	items->head = 0;
	items->tail = 0;
	items->size = 0;
	items->display = d;
	items->free = f;

	return items;
}

/* insert method */
/* This is the method used to insert items into the DLL. */ 
void *insertDLL(DLL *items,int index,void *value) {
	DLLNODE *newNode = newNODE(value);
	DLLNODE *cur = 0;

	assert((index >= 0) && (index <= items->size));

	if (items->size == 0) {					// Empty list
		items->head = newNode;
		(items->head)->next = 0;
		(items->head)->previous = 0;
		items->tail = newNode;
	}

	else {							// Non-empty list
		if (index == items->size) {			// Insert tail
//			cur = search(index - 1, items->head);
			insertLast(items, newNode, items->tail);
		}	

		else if (index == 0) {				// Insert head
			cur = search(index - 1, items->head);
			insertFirst(items, newNode, cur);
		}

		else {
			if (items->size / index < 2) {
		        	cur = searchReverse(index - 1, items->size - 1, items->tail);		
			}

			else {					// index near head
				cur = search(index - 1, items->head);
			}

			insertMiddle(newNode, cur);		// Insert middle
		}
	}	
	
	++items->size;

	return newNode;
}

/* remove method */
/* This is the method used to remove items from the DLL. */
void *removeDLL(DLL *items,int index) {
	int i = 0;

	// TODO: CHANGE TO DLLNODE *cur = getHead(items);
	// TODO: IN GENERAL, UPDATE TO SETTERS AND GETTERS
	DLLNODE *cur = items->head;
	DLLNODE *oldNode = 0;
	void *old = 0;

	// TODO: GET SIZE, STORE IN VARIABLE, AND ASSERT
	assert(items->size > 0);
	assert((index >= 0) && (index < items->size));

	if (index == 0) {					// Remove head
		old = (items->head)->value;
		oldNode = items->head;
		items->head = (items->head)->next;
		
		if (items->size == 1) {				// One node
			items->tail = 0;
		}

		else {
			(items->head)->previous = 0;
		}
	}

	else {							// Remove elsewhere
		if (items->size / index < 2) {
			cur = searchReverse(index, items->size - 1, items->tail);
		}
		
		else {
			while (i < index) {
				++i;
				cur = cur->next;
			}
		}
			old = cur->value;
			oldNode = cur;
			(cur->previous)->next = cur->next;

			if (cur->next == 0) {				// Remove tail
				items->tail = cur->previous;
			}
		
			else {
				(cur->next)->previous = cur->previous;	// Remove middle
			}
	}

	free(oldNode);

	--items->size;

	return old;
}

/* union method */
/* This is the method used to union two DLL objects. */
void unionDLL(DLL *recipient,DLL *donor) {
	int sizeDonor = sizeDLL(donor);
	int sizeRecip = sizeDLL(recipient);

	if (sizeDonor == 0) {
		return;
	}

	else if (sizeRecip == 0) {
		recipient->head = donor->head;
		recipient->tail = donor->tail;
		recipient->size = donor->size;

		donor->head = donor->tail = 0;
		donor->size = 0;

		return;
	}

	(recipient->tail)->next = donor->head;
	(donor->head)->previous = recipient->tail;
	recipient->tail = donor->tail;
	recipient->size += donor->size;
	
	donor->head = donor->tail = 0;
	donor->size = 0;

	return;
}

/* accessor */
/* This is the accessor for the DLL class. */
void *getDLL(DLL *items,int index) {
	DLLNODE *cur = 0;

	assert((index >= 0) && (index < items->size));

	if (index == items->size - 1) {
		return (items->tail)->value;
	}

	if (index == 0) {
		return (items->head)->value;
	}

	if (items->size / index < 2) {
		cur = searchReverse(index, items->size - 1, items->tail);
	}

	else {
		cur = search(index, items->head);
	}

	return cur->value;					// Value not found
}

/* mutator */
/* This is the mutator for the DLL class. */
void *setDLL(DLL *items,int index,void *value) {
	DLLNODE *cur = 0;

	assert((index >= 0) && (index <= items->size));

	if (index == items->size) {				// Set after tail
		insertDLL(items, index, value);

		return NULL;
	}

	else if (index == items->size - 1) {
		void *old = (items->tail)->value;

		(items->tail)->value = value;

		return old;
	}

	else if (index == 0) {
		cur = items->head;

		void *old = cur->value;

		cur->value = value;

		return old;
	}

	if (items->size / index < 2) {
		cur = searchReverse(index, items->size - 1, items->tail);
	}

	else {
		cur = search(index, items->head);
	}

	void *old = cur->value;					// Store old value

	cur->value = value;					// Set new value

	return old;
}

/* DLL size */
/* This method returns the size of the DLL. */
int sizeDLL(DLL *items) {
	return items->size;
}


/* display method */
/* This method displays the DLL using the passed
display method. */
void displayDLL(DLL *items,FILE *fp) {
	DLLNODE *cur = items->head;

	fprintf(fp, "{{");

	while (cur) {						// Traverse list
		items->display(cur->value, fp);			// Display nodes

		if ((items->size > 1) && (cur->next)) {
			fprintf(fp, ",");
		}

		cur = cur->next;				// Update pointer
	}

	fprintf(fp, "}}");
	return;
}

/* debugged display method */
/* This method displays the DLL using the passed 
display method in the correct format for a DLL. */
void displayDLLdebug(DLL *items,FILE *fp) {
	DLLNODE *cur = items->head;

	fprintf(fp, "head->{{");					// Print head

	while (cur) {						// Traverse list
		items->display(cur->value, fp);			// Display nodes

		if ((items->size > 1) && (cur->next)) {
			fprintf(fp, ",");
		}

		cur = cur->next;				// Update pointer
	}

	fprintf(fp, "}},tail->{{");				// Print tail

	if (items->tail) {
		items->display((items->tail)->value, fp);
	}

	fprintf(fp, "}}");

	return;
}

/* free method */
/* This method traverses the DLL and frees
the generic values, as well as the nodes that hold. 
If null is returned, the generic value isn't found. */
void freeDLL(DLL *items) {
	if (items->free != NULL) {
		while (items->size > 0) {
			void *value = removeDLL(items, 0);
	
			items->free(value);
		}
	}

	else {
		while (items->size > 0) {
			removeDLL(items, 0);
		}
	}

	free(items);
	
	return;
}

/* removeDLLall Method */
/* Sets head, tail, and size to 0. Frees nodes, not values. */
 
void removeDLLall(DLL *items) {
	int size = sizeDLL(items);
	DLLNODE *head = getHead(items);
	
	while (size > 0) {					// Free list nodes
		removeDLLnode(items, head);
		
		size = sizeDLL(items);
		head = getHead(items);
	}

	// DLL size now 0

	setHead(items, 0);					// Set head NULL	
	setTail(items, 0);					// Set tail NULL

	return;
}

/* removeDLLnode Method */
/* Removes given node from DLL. */

void *removeDLLnode(DLL *items, void *n) {
	DLLNODE *node = n;					// void * is DLLNODE *
	DLLNODE *head = getHead(items);
	DLLNODE *tail = getTail(items);
	DLLNODE *iterate = 0;
	DLLNODE *iterateNext = 0;
	DLLNODE *iteratePrevious = 0;
	DLLNODE *oldNode = 0;
	void *old = 0;
	int size = sizeDLL(items);
	
	assert(size > 0);
	assert(node != 0);

	setIterator(items, node);				// Update iterator
	iterate = getIterator(items);				// Get iterator
	iterateNext = getDLLNODEnext(iterate);
	iteratePrevious = getDLLNODEprevious(iterate);
	

	if (iterate == head) {
		old = getDLLNODEvalue(iterate);			// Get head value
		oldNode = iterate;					
		setHead(items, iterateNext);
		head = getHead(items);				// Update head

		if (size == 1) setTail(items, 0);		// One node
		
		else setDLLNODEprevious(head, 0);
	}

	else if (iterate == tail) {
		old = getDLLNODEvalue(iterate);			// Get tail value
		oldNode = iterate;
		setTail(items, iteratePrevious);
		tail = getTail(items);

		if (size == 1) setTail(items, 0);
	
		else setDLLNODEnext(tail, 0);
	}
	
	else {
		old = getDLLNODEvalue(iterate);			// Get value
		oldNode = iterate;
		setDLLNODEnext(iteratePrevious, iterateNext);

		if (iterateNext == 0) setTail(items, iteratePrevious);
		
		else setDLLNODEprevious(iterateNext, iteratePrevious);
	}
	
	free(oldNode);

	setDLLsize(items, size - 1);

	return old;	
}

/* firstDLL Method */
/* Starts a DLL iterator, setting object to point
to head of DLL. */

void firstDLL(DLL *items) {
	DLLNODE *head = getHead(items);	

	setIterator(items, head);				// Set iterator to head	

	return;
}

/* secondDLL Method */
/* Starts a DLL iterator, setting object to point
to tail of DLL. */

void lastDLL(DLL *items) {
	DLLNODE *tail = getTail(items);

	setIterator(items, tail);				// Set iterator to tail
	
	return;
}

/* moreDLL Method */
/* Returns 0 if iterator == NULL, 1 otherwise. */

int moreDLL(DLL *items) {
	DLLNODE *iterate = getIterator(items);

	return (iterate == 0) ? 0 : 1;
}

/* nextDLL Method */
/* Sets iterator to next node in list. */

void nextDLL(DLL *items) {
	DLLNODE *iterate = getIterator(items);
	DLLNODE *next = getDLLNODEnext(iterate);

	setIterator(items, next);				// Set iterator to next

	return;
}

/* prevDLL Method */
/* Sets iterator to previous node in list. */

void prevDLL(DLL *items) {
	DLLNODE *iterate = getIterator(items);
	DLLNODE *previous = getDLLNODEprevious(iterate);

	setIterator(items, previous);				// Set iterator to previous

	return;
}

/* currentDLL Method */
/* Returns value at current node. */

void *currentDLL(DLL *items) {
	DLLNODE *iterate = getIterator(items);
	void *value = 0;

	value = getDLLNODEvalue(iterate);

	return value;
}

/*************** private implementations ***************/

/* DLLNODE constructor */
/* This is the constructor for the DLLNODE object. */
static DLLNODE *newNODE(void *val) {
	DLLNODE *node = malloc(sizeof(DLLNODE));

	assert(node != 0);
	node->value = val;
	node->next = 0;
	node->previous = 0;

	return node;
}

static void setDLLsize(DLL *items, int s) {
	items->size = s;

	return;
}

/* setDLLNODE Method */
/* Sets DLLNODE to given node. */

/*
static void setDLLNODE(DLLNODE *c, DLLNODE *n) {
	c = n;

	return;
}*/

/* getDLLNODE Method */
/* Returns DLLNODE as void * pointer. */

/*
static void *getDLLNODE(DLLNODE *n) {
	return n;
}*/

/* setIterator Method */
/* Sets private iterator to given node. */

static void setIterator(DLL *items, DLLNODE *n) {
	items->iterator = n;

	return;
}

/* getIterator Method */
/* Returns DLL iterator. */

static void *getIterator(DLL *items) {
	return items->iterator;
}

/* setDLLNODEvalue Method */
/* Sets value of DLLNODE. */

/*
static void setDLLNODEvalue(DLLNODE *n, void *v) {
	if (v != 0) n->value = v;

	return;
}*/

/* getDLLNODEvalue Method */
/* Gets value of DLLNODE. */

static void *getDLLNODEvalue(DLLNODE *n) {
	return n->value;
}

/* setDLLNODEnext Method */
/* Sets DLLNODE next pointer. */

static void setDLLNODEnext(DLLNODE *node, DLLNODE *next) {
	node->next = next;

	return;
}

/* getDLLNODEnext Method */
/* Gets DLLNODE next pointer. */

static DLLNODE *getDLLNODEnext(DLLNODE *n) {
	return n->next;
}

/* setDLLNODEprevious Method */
/* Sets DLLNODE previous pointer. */


static void setDLLNODEprevious(DLLNODE *n, DLLNODE *p) {
	n->previous = p;

	return;	
}

/* getDLLNODEprevious Method */
/* Gets DLLNODE previous pointer. */

static DLLNODE *getDLLNODEprevious(DLLNODE *n) {
	return n->previous;
}

/* setHead Method */
/* Sets head of DLL to given node. */


static void setHead(DLL *items, DLLNODE *node) {
	items->head = node; 

	return;
}

/* getHead Method */
/* Gets head of DLL. */

static DLLNODE *getHead(DLL *items) {
	return items->head;
}

/* setTail Method */
/* Sets tail of DLL to given node. */


static void setTail(DLL *items, DLLNODE *node) {
	items->tail = node;

	return;
}

/* getTail Method */
/* Gets tail of DLL. */

static DLLNODE *getTail(DLL *items) {
	return items->tail;
}

/* search method */
/* This method searches for a given node in a
DLL object.*/
static DLLNODE *search(int index,DLLNODE *node) {
	int i = 0;
	
	while (i < index) {
        	++i;
		node = node->next;
	}

	return node;
}

/* search reverse method */
/* Searches for a given node in reverse in a
DLL object.*/
static DLLNODE *searchReverse(int index,int size,DLLNODE *node) {
	int i = 0;

	while (i < size - index) {
		++i;	
		node = node->previous;
	}

	return node;
}

/* insertLast method */
/* Inserts at the tail of the DLL. */
static void insertLast(DLL *items,DLLNODE *newNode,DLLNODE *current) {
	newNode->next = current->next;
	newNode->previous = current;
                         
	if (current->next == 0) {
        	items->tail = newNode;
        }
 
        else {
        	(current->next)->previous = newNode;
        }
               
        current->next = newNode;

	return;
}

/* insertFirst method */
/* Inserts at the head of the DLL. */
static void insertFirst(DLL *items,DLLNODE *newNode,DLLNODE *current) {
	newNode->next = current;
        newNode->previous = current->previous;
                         
        if (current->previous == 0) {
        	items->head = newNode;
        }
                         
        else {
        	(current->previous)->next = newNode;
        }
                  
        current->previous = newNode;

	return;
}

/* insertMiddle method */
/* Inserts in the middle of the DLL. */
static void insertMiddle(DLLNODE *newNode,DLLNODE *current) {
	newNode->next = current->next;             
	newNode->previous = current;
	(current->next)->previous = newNode;
        current->next = newNode;

	return;
}

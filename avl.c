/* Author: Alan Manning*/
/* Class: CS201 Data Structures and Algorithms */
/* Assignment 2 */
/* Date: 3/8/2018 */

/* This is the implementation file for the header file "avl.h", which is used to create an avl tree 'class' for generic data types.*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <stdbool.h>
#include "bst.h"
#include "avl.h"

#define LEFT 1							// Left-heavy
#define BALANCED 0						// Balanced
#define RIGHT -1						// Right-heavy

// NODE Definition
typedef struct AVLVALUE AVLV;

struct AVLVALUE {
	int leftHeight;						// AVLV leftHeight
	int rightHeight;					// AVLV rightHeight
	int height;						// AVLV height
	int factor;						// Balance factor
	int count;						// duplicate count
	void *value;						// AVLV value
	void (*disp)(void *,FILE *);				// display method
	int (*compare)(void *,void *);				// compare method
	void (*free)(void *);					// free method
};

// avl struct definition

struct avl {
	void (*disp)(void *, FILE *);				// display field
	int (*compare)(void *, void *);				// compare field
	void (*free)(void *);					// free method
	int size;						// tree size
	BST *binTree;						// Associated bst
	BSTNODE *rootNode;					// BST root
};

/*************** private interface ***************/
static BST *getBST(AVL *);
static AVLV *newAVLV(
	void (*)(void *,FILE *),
	int (*)(void *,void *),
	void (*)(void *),
	void *);
static void setAVLVvalue(AVLV *,void *);
static void *getAVLVvalue(AVLV *);
static void setAVLVcount(AVLV *,int);
static int getAVLVcount(AVLV *);
static void setAVLVfactorL(AVLV *);
static void setAVLVfactorR(AVLV *);
static void setAVLVfactorB(AVLV *);
static int getAVLVfactor(AVLV *);
static void setAVLVheight(AVLV *,int);
static int getAVLVheight(AVLV *);
static void setAVLVLHeight(AVLV *,int);
static void setAVLVRHeight(AVLV *,int);
static int getAVLVLHeight(AVLV *);
static int getAVLVRHeight(AVLV *);
static void adisplay(void *,FILE *);
static int compareAVLV(void *,void *);
static void swapper(BSTNODE *,BSTNODE *);
static void freeAVLV(void *);
static BSTNODE *sibling(BSTNODE *);
static void setBalance(AVL *, AVLV *);
static bool linear(AVL *,AVLV *);
static BSTNODE *favorite(BSTNODE *);
static void insertionFixup(AVL *,BSTNODE *);
static void deleteFixup(AVL *,BSTNODE *);
static void increaseAVLVcount(AVLV *);
static void decreaseAVLVcount(AVLV *);
static void setAVLsize(AVL *,int);
static bool isLeftChild(BSTNODE *);
static bool isRightChild(BSTNODE *);
static void rotateRight(BST *,BSTNODE *,BSTNODE *);
static void rotateLeft(BST *,BSTNODE *,BSTNODE *);
static void rotate(BST *,BSTNODE *,BSTNODE *);
static int sizeAVLactual(AVL *);

/*************** AVL public definitions ***************/

/* AVL Constructor */

AVL *newAVL(
	void (*display)(void *, FILE *),
	int (*comparator)(void *, void *),
	void (*freer)(void *)) {
	AVL *tree = malloc(sizeof(AVL));			// Allocate space for AVL object

	assert(tree != 0);

	tree->size = 0;						// Set AVL size to 0
	tree->disp = display;					// disp field = display method
	tree->compare = comparator;				// compare field = comparator method
	tree->free = freer;					// free field = freer method
								// binTree field = newBST
	tree->binTree = newBST(adisplay, compareAVLV, swapper, freeAVLV);

	return tree;
}

/* insertAVL Method */
/* Inserts into AVL tree. */

void insertAVL(AVL *t, void *v) {
	int count = 0;
	BST *tree = getBST(t);
	int bstSize = sizeBST(tree);
	int avlSize = sizeAVLactual(t);
	BSTNODE *result = NULL;
	BSTNODE *r = NULL;
	AVLV *a = NULL;
	AVLV *searchA = newAVLV(t->disp, t->compare, t->free, v);
	AVLV *foundA = NULL;

	count = findAVLcount(t, v);				// Determine count

	if (count == 0) {
		a = newAVLV(t->disp, t->compare, t->free, v);
		increaseAVLVcount(a);				// Update count

		//TODO: MAKE SURE LINES 134-138 ARE OK
		insertBST(tree, a);				// Insert AVLV into BST

		if (bstSize == 0) {				// Inserted root
			r = getBSTroot(tree);
			setBSTNODEparent(r, r);			// Root's parent is itself
			bstSize = sizeBST(tree);
		}
	
		BSTNODE *x = findBST(tree, a);
	
		insertionFixup(t, x);

		setAVLsize(t, avlSize + 1);

		free(searchA);
	
		return;
	}

	result = findBST(tree, searchA);		// Find BSTNODE with AVLV

	foundA = getBSTNODEvalue(result);		// Get AVLV node
	
	increaseAVLVcount(foundA);			// Update count

	setAVLsize(t, avlSize + 1);			// Update AVL size

	free(searchA);
		
//	t->disp(getAVLVvalue(a), stdout);
//	printf("\n");

	return;
}

/* findAVLcount Method */
/* Returns frequency of searched value; 
   else NULL. */

int findAVLcount(AVL *t, void *v) {
	int count = 0;
	BSTNODE *result = NULL;
	BST *tree = getBST(t);
	AVLV *a = NULL;
	AVLV *searchA = newAVLV(t->disp, t->compare, t->free, v);

	result = findBST(tree, searchA);			// Find correct BSTNODE
	
	if (result == NULL) {
		free(searchA);					// Free extra node
		return 0;
	}

	a = getBSTNODEvalue(result);				// Get AVLV value

	count = getAVLVcount(a);				// Update count
	
	free(searchA);						// Free extra node

	return count;
}

/* findAVL Method */
/* Returns searched-for value; else NULL. */

void *findAVL(AVL *t, void *v) {
	BST *tree = getBST(t);
	BSTNODE *result = NULL;
	AVLV *a = NULL;
	AVLV *searchA = newAVLV(t->disp, t->compare, t->free, v);
	void *value = NULL;

	result = findBST(tree, searchA);			// Call findBST

	if (result == NULL) {
//		printf("Value ");
//		t->disp(getAVLVvalue(searchA), stdout);
//		printf(" not found\n");
		free(searchA);					// Free extra node
		return NULL;					// Not found
	}

	a = getBSTNODEvalue(result);				// Get AVLV node
	
	value = getAVLVvalue(a);				// Get actual value

	free(searchA);						// Free extra value

	return value;
}

/* deleteAVL Method */
/* Returns deleted value. If count = 1,
   node removed from tree. */

void *deleteAVL(AVL *t, void *v) {
	int count = 0;
	BST *tree = getBST(t);
	int bstSize = sizeBST(tree);
	int avlSize = sizeAVLactual(t);
	AVLV *searchA = newAVLV(t->disp, t->compare, t->free, v);
	AVLV *foundA = NULL;
	BSTNODE *result = NULL;
	BSTNODE *deleted = NULL;
	AVLV *value = NULL;
	void *actualValue = NULL;

	count = findAVLcount(t, v);				// Determine count
	result = findBST(tree, searchA);			// Determine value
	
	if (result == NULL) {
		free(searchA);					// Free extra node
		return NULL;					// Node not in AVL
	}

	foundA = getBSTNODEvalue(result);			// Get AVLV node

	if (count > 1) {
		decreaseAVLVcount(foundA);			// Update count
		setAVLsize(t, avlSize - 1);
		free(searchA);					// Free extra node
		return NULL;
	}

	decreaseAVLVcount(foundA);				// Update count
	
	deleted = swapToLeafBST(tree, result);			// Swap to Leaf

	value = getBSTNODEvalue(deleted);			// Get AVLV node

	actualValue = getAVLVvalue(value);			// Get actual value

	deleteFixup(t, deleted);				// Call deleteFixup

	pruneLeafBST(tree, deleted);				// Prune from tree

	setAVLsize(t, avlSize - 1);				// Update AVL size
	setBSTsize(tree, bstSize - 1);				// Update BST size

	free(searchA);						// Free extra nodes
	free(value);
	free(deleted);

	return actualValue;
}

/* sizeAVL Method */
/* Returns size of AVL. */

int sizeAVL(AVL *t) {
	BST *tree = getBST(t);
	int size = sizeBST(tree);

	return size;
}

/* duplicates Method */
/* Returns number of tree duplicates. */

int duplicatesAVL(AVL *t) {
	BST *tree = getBST(t);

	int duplicates = sizeAVLactual(t) - sizeBST(tree);	// No. of duplicates

	return duplicates;
}

/* statisticsAVL Method */
/* Displays number of duplicates and 
   BST tree statistics. */

void statisticsAVL(AVL *t, FILE *fp) {
	int repeats = duplicatesAVL(t);
	BST *tree = getBST(t);

	fprintf(fp, "Duplicates: %d\n", repeats);		// Prints duplicates
	statisticsBST(tree, fp);				// Prints BST stats
		
	return;
}

/* displayAVL Method */
/* Calls underlying displayBSTdecorated method. */

void displayAVL(AVL *t, FILE *fp) {
	BST *tree = getBST(t);
	int size = sizeBST(tree);

	if (size > 0) {
		displayBSTdecorated(tree, fp);				// Call displayBSTdecorated
	}

	else {
		fprintf(fp, "EMPTY\n");
	}

	return;
}

/* displayAVLdebug Method */
/* Calls underlying displayBST method. */

void displayAVLdebug(AVL *t, FILE *fp) {
	BST *tree = getBST(t);

	displayBST(tree, fp);					// Call underlying display

	return;
}

/* freeAVL Method */
/* Frees AVL. */

void freeAVL(AVL *t) {
	BST *tree = getBST(t);

	freeBST(tree);						// Free BST and AVLV nodes

	free(t);						// Free AVL

	return;
}

/*************** Private Method Definitions ***************/

/* AVLV Node Constructor */

static AVLV *newAVLV(
	void (*d)(void *, FILE *),
	int (*c)(void *, void *),
	void (*f)(void *),
	void *value) {
	AVLV *avlValue = malloc(sizeof(AVLV));			// Allocate space
	
	assert(avlValue != NULL);

	avlValue->value = value;
	avlValue->leftHeight = 0;				// Zero leftHeight
	avlValue->rightHeight = 0;				// Zero rightHeight
	avlValue->height = 1;					// Leaf height = 0
	avlValue->factor = BALANCED;				// Leaf balanced
	avlValue->count = 0;					// Initialize count
	avlValue->disp = d;					// disp field = display method
	avlValue->compare = c;					// compare field = compare method
	avlValue->free = f;					// free field = free method

	return avlValue;
}

static BST *getBST(AVL *t) {
	BST *tree = t->binTree;

	if (tree == NULL) return NULL;

	return tree;	
}

static int compareAVLV(void *a, void *b) {
	AVLV *x = a;						// Values are AVLV nodes
	AVLV *y = b;

	void *p = getAVLVvalue(x);				// Get AVLV value fields
	void *q = getAVLVvalue(y);

	return x->compare(p, q);				// Use AVLV compare
}

static void swapper(BSTNODE *a, BSTNODE *b) {
	AVLV *ta = getBSTNODEvalue(a);
	AVLV *tb = getBSTNODEvalue(b);

	// swap the values stored in the AVL objects
	void *vtemp = getAVLVvalue(ta);
	void *bValue = getAVLVvalue(tb);
	setAVLVvalue(ta, bValue);
	setAVLVvalue(tb, vtemp);

	// swap the counts stored in the AVL value objects
	int ctemp = getAVLVcount(ta);
	int bCount = getAVLVcount(tb);
	setAVLVcount(ta, bCount);
	setAVLVcount(tb, ctemp);

	// note: AVL heights and balance factors are NOT swapped	
}

static void adisplay(void *v, FILE *fp) {
	AVLV *w = v;
	void *value = getAVLVvalue(w);				// Value is AVLV node
//	int height = getAVLVheight(w);
	int count = getAVLVcount(w);
	int factor = getAVLVfactor(w);
	w->disp(value, fp);					// Use AVLV display

	if (count > 1) {
		fprintf(fp, "[%d]", count);			// Show AVLV count
	}

	if (factor != 0) {					// Show AVLV factor
		if (factor == -1) {
			fprintf(fp, "-");
		}

		else {
			fprintf(fp, "+");
		}
	}

//	fprintf(fp, " %d(height) %d(factor)", height, factor);	// TEST

	return;
}

/* freeAVLV Method */
/* Frees AVLV node and value. */

static void freeAVLV(void *v) {
	AVLV *oldAVLV = v;
	void *old = getAVLVvalue(oldAVLV);

	if (oldAVLV->free != NULL) {
		oldAVLV->free(old);				// Free node value
	}

	free(oldAVLV);						// Free AVLV node
	
	return;
}

/* setAVLVvalue Method */
/* Sets AVLV value. */

static void setAVLVvalue(AVLV *a, void *v) {
	a->value = v;

	return;
}

/* getAVLVvalue Method */
/* Returns AVLV value. */

static void *getAVLVvalue(AVLV *a) {
	return a->value;
}

/* setAVLVcount Method */
/* Sets AVLV count. */

static void setAVLVcount(AVLV *a, int c) {
	a->count = c;
	
	return;
}

/* getAVLVcount Method */
/* Returns AVLV count. */

static int getAVLVcount(AVLV *a) {
	return a->count;
}

/* setAVLVfactorL Method */
/* Sets AVLV factor LEFT. */

static void setAVLVfactorL(AVLV *a) {
	a->factor = LEFT;
	
	return;
}

/* setAVLVfactorR Method */
/* Sets AVLV factor RIGHT. */

static void setAVLVfactorR(AVLV *a) {
	a->factor = RIGHT;

	return;
}

/* setAVLVfactorB Method */
/* Sets AVLV factor BALANCED. */

static void setAVLVfactorB(AVLV *a) {
	a->factor = BALANCED;
	
	return;
}


/*static void setAVLVfactor(AVLV *a, int factor) {
	a->factor = factor;

	return;
}*/

/* getAVLVfactor Method */
/* Gets AVLV factor. */

static int getAVLVfactor(AVLV *a) {
	return a->factor;
}

/* setAVLVheight Method */
/* Sets AVLV height */

static void setAVLVheight(AVLV *a, int height) {
	a->height = height;

	return;
}

/* getAVLVheight Method */
/* Returns AVLV height. */

static int getAVLVheight(AVLV *a) {
	return a->height;
}

/* setAVLVLHeight Method */
/* Sets AVLV leftHeight. */

static void setAVLVLHeight(AVLV *a, int lH) {
	a->leftHeight = lH;

	return;
}

/* getAVLVLHeight method */
/* Gets AVLV leftHeight. */

static int getAVLVLHeight(AVLV *a) {
	return a->leftHeight;
}

/* setAVLVRHeight Method */
/* Sets AVLV rightHeight. */

static void setAVLVRHeight(AVLV *a, int rH) {
	a->rightHeight = rH;

	return;
}

static int getAVLVRHeight(AVLV *a) {
	return a->rightHeight;
}

/* increaseAVLVcount Method */
/* Increases AVLV count. */

static void increaseAVLVcount(AVLV *a) {
	int count = getAVLVcount(a);

	setAVLVcount(a, count + 1);

	return;
}

/* decreaseAVLVcount Method */
/* Decreases AVLV count. */

static void decreaseAVLVcount(AVLV *a) {
	int count = getAVLVcount(a);

	setAVLVcount(a, count - 1);

	return;
}

/* setAVLsize Method */
/* Updates AVL size. */

static void setAVLsize(AVL *t, int size) {
	t->size = size;

	return;
}

/* setBalance Method */
/* Sets balance factor for given AVLV node. */

static void setBalance(AVL *t, AVLV *a) {
	BST *tree = getBST(t);
	BSTNODE *result = findBST(tree, a);
	
	if (result == NULL) return;

	BSTNODE *leftChild = getBSTNODEleft(result);
	BSTNODE *rightChild = getBSTNODEright(result);
	AVLV *lValue = NULL;
	AVLV *rValue = NULL;
	int lcHeight = 0;
	int rcHeight = 0;
	int maxHeight = 0;
	int factor = 0;
	
	// Set a's leftHeight to height of left child, 0 if none
	if (leftChild != NULL) {
		lValue = getBSTNODEvalue(leftChild);
		lcHeight = getAVLVheight(lValue);
	}

	setAVLVLHeight(a, lcHeight);

	// Set a's rightHeight to height of right child, 0 if none
	if (rightChild != NULL) {
		rValue = getBSTNODEvalue(rightChild);
		rcHeight = getAVLVheight(rValue);
	}

	setAVLVRHeight(a, rcHeight);

	// Set a's height to max of leftHeight and rightHeight, plus 1

	maxHeight = lcHeight;

	if (lcHeight < rcHeight) maxHeight = rcHeight;

	setAVLVheight(a, maxHeight + 1);			// Set node height

	factor = lcHeight - rcHeight;				// Update balance factor
//	printf("%d\n", maxHeight + 1);

	if (factor >= 1) setAVLVfactorL(a);
	else if (factor <= -1) setAVLVfactorR(a);
	else setAVLVfactorB(a);
//	setAVLVfactor(a, factor);
//	setAVLVfactor(a, factor);

	return;
}

/* sibling Method */
/* Returns AVLV sibling of AVLV node. */

static BSTNODE *sibling(BSTNODE *x) {
	if (x == NULL) return NULL;				// Node not found
//	AVLV *a = getBSTNODEvalue(x);
//	AVLV *lv = NULL;
//	AVLV *rv = NULL;
//	void *value = getAVLVvalue(a);

	BSTNODE *parent = getBSTNODEparent(x);

	if (parent == NULL) return NULL;			// Root node

	BSTNODE *leftChild = getBSTNODEleft(parent);
	BSTNODE *rightChild = getBSTNODEright(parent);

	// If a is parent's left child, return parent's right child
	if (leftChild != NULL) {
//		lv = getBSTNODEvalue(leftChild);
		
		if (leftChild == x) {
//		printf("here!\n");
//		displaySTRING(getAVLVvalue(getBSTNODEvalue(leftChild)), stdout);
//		printf("\n");
			if (rightChild != NULL) {
				return rightChild;
			}
			 
		//	return NULL;				// Only child
		}
	}

//	if (rightChild == NULL) return NULL;

	// If a is parent's right child, return parent's left child	
	if (rightChild != NULL) {
//		displaySTRING(getAVLVvalue(getBSTNODEvalue(rightChild)), stdout);
//		printf("\n");
//		rv = getBSTNODEvalue(rightChild);
		if (rightChild == x) {
			if (leftChild != NULL) {
		//printf("here!\n");
				return leftChild;
			}
			 
		//	return NULL;				// Only child
		}
	}
	
	return NULL;
}

/* linear Method */
/* Returns true if parent and child both left or right children. */

static bool linear(AVL *t, AVLV *a) {
	bool linear = false;
	BST *tree = getBST(t);
	BSTNODE *node = findBST(tree, a);
	BSTNODE *p = NULL;
	BSTNODE *gp = NULL;
	BSTNODE *pLeft = NULL;
	BSTNODE *pRight = NULL;
	BSTNODE *gpLeft = NULL;
	BSTNODE *gpRight = NULL;
	
	if (node == NULL) return linear;			// Node not in AVL
	
	p = getBSTNODEparent(node);

	if (p == NULL) return linear;				// Root node

	gp = getBSTNODEparent(p);

	if (gp == NULL) return linear;				// No gp
	//printf("Hi\n");

	pLeft = getBSTNODEleft(p);
	pRight = getBSTNODEright(p);
	gpLeft = getBSTNODEleft(gp);
	gpRight = getBSTNODEright(gp); 

	if ((pLeft != NULL) && (gpLeft != NULL)) {
		if ((gpLeft == p) && (pLeft == node)) {
			linear = true;
		}
	}

	if ((pRight != NULL) && (gpRight != NULL)) {
		if ((gpRight == p) && (pRight == node)) {
			linear = true;
		}
	}

	return linear;
}

/* favorite Method */
/* Returns favorite child. */

static BSTNODE *favorite(BSTNODE *node) {
	AVLV *a = getBSTNODEvalue(node);
	int favorite = getAVLVfactor(a);
//	int favorite = 0;
	int leftChildH = 0;
	int rightChildH = 0;
	BSTNODE *leftChild = NULL;
	BSTNODE *rightChild = NULL;
	
	if (node == NULL) return 0;				// Node not found
		
	leftChild = getBSTNODEleft(node);
	rightChild = getBSTNODEright(node);

//	if (leftChild == NULL) printf("Left bad\n");
//	if (rightChild == NULL) printf("Right bad\n");

	if (leftChild != NULL) {
//		leftChildH = getAVLVheight(getBSTNODEvalue(leftChild));
		leftChildH = getAVLVLHeight(a);
//		printf("Yay%d\n", leftChildH);
	}

	if (rightChild != NULL) {
//		rightChildH = getAVLVheight(getBSTNODEvalue(rightChild));
		rightChildH = getAVLVRHeight(a);
//		printf("uhhh...\n");
	}

	favorite = leftChildH - rightChildH;
//	printf("favorite: %d\n", favorite);

	// Left child favored
	if ((favorite > 0) && (leftChild != NULL)) {
		//printf("Left\n");
		return leftChild;
	}

	// Right child favored
	else if ((favorite < 0) && (rightChild != NULL)) {
//		printf("Testing\n");
		return rightChild;
	}
	

	return NULL;						// No favorite
}

static void insertionFixup(AVL *t, BSTNODE *x) {
	BST *tree = getBST(t);
	BSTNODE *r = NULL;
	BSTNODE *p = NULL;
	AVLV *y = NULL;
	AVLV *pValue = NULL;
	// loop while true
	while (1) {
		AVLV *a = getBSTNODEvalue(x);
		r = getBSTroot(tree);
		p = getBSTNODEparent(x);
		if (p != NULL) pValue = getBSTNODEvalue(p);
		
		// x is the root
		if (x == r) {
			return;
		}	

		// favorite(a.parent) == sibling(a)
		else if ((p != NULL) && (sibling(x) != NULL) && (favorite(p) == sibling(x))) {
			// set balance of parent
			setBalance(t, pValue);

			return;
		}

		// Parent is balanced
		else if ((p != NULL) && (pValue != NULL) && (favorite(p) == NULL)) {
			// set balance of parent
			setBalance(t, pValue);
			x = p;
			continue;
		}

		// Parent is unbalanced
		else {
			BSTNODE *yNode = favorite(x);
			if (yNode != NULL ) y = getBSTNODEvalue(yNode);
			
			if ((yNode != NULL) && (!linear(t, y))) {
				// rotate y to x
				rotate(tree, yNode, r);
				// rotate y to p
				rotate(tree, yNode, r);
				// set balance of x, p, and y
				a = getBSTNODEvalue(x);
				setBalance(t, a);
				pValue = getBSTNODEvalue(p);
				setBalance(t, pValue);
				y = getBSTNODEvalue(yNode);
				setBalance(t, y);
			}

			// y, x, p are linear or no favorite
			else {
				// rotate x to p
				rotate(tree, x, r);
				// set balance of p
				pValue = getBSTNODEvalue(p);
				setBalance(t, pValue);
				// set balance of x
				a = getBSTNODEvalue(x);
				setBalance(t, a);
			}
			// exit loop
			return;
		}
	}
		

	return;
}

/*static bool isBalanced(BSTNODE *node) {
	bool balanced = false;
	int lHeight = 0;
	int rHeight = 0;
	int result = 0;
	AVLV *l = NULL;
	AVLV *r = NULL;
	BSTNODE *leftChild = getBSTNODEleft(node);
	BSTNODE *rightChild = getBSTNODEright(node);

	if (leftChild != NULL) {
		l = getBSTNODEvalue(leftChild);
		lHeight = getAVLVheight(l);
	}

	else if (rightChild != NULL) {
		r = getBSTNODEvalue(rightChild);
		rHeight = getAVLVheight(r);
	}

	result = lHeight - rHeight;

	if ((result <= 1) || (result >= -1)) {
		balanced = true;
	}

	return balanced;
}*/

static bool isLeftChild(BSTNODE *node) {
	bool leftChild = false;

	if (node == NULL) return leftChild;

	BSTNODE *nodeParent = getBSTNODEparent(node);
	BSTNODE *parentLeft;

	if (nodeParent != NULL) {
		parentLeft = getBSTNODEleft(nodeParent);
	}

	if (parentLeft == node) {
		leftChild = true;
	}
	
	return leftChild;
}

static bool isRightChild(BSTNODE *node) {
	bool rightChild = false;

	if (node == NULL) return rightChild;

	BSTNODE *nodeParent = getBSTNODEparent(node);
	BSTNODE *parentRight;

	if (nodeParent != NULL) {
		parentRight = getBSTNODEright(nodeParent);
	}

	if (parentRight == node) {
		rightChild = true;
	}

	return rightChild;
}

static void rotate(BST *t, BSTNODE *first, BSTNODE *root) {
	if (isLeftChild(first)) {
		rotateRight(t, first, root);
	} 

	else {
		rotateLeft(t, first, root);
	}

	return;
}

static void rotateRight(BST *t, BSTNODE *node, BSTNODE *root) {
	if (node == NULL) return;

	BSTNODE *oldParent = getBSTNODEparent(node);
	BSTNODE *gp = NULL;
	
	if (oldParent != NULL) {
		gp = getBSTNODEparent(oldParent);		// Old gp, possibly NULL
	}

	BSTNODE *rightChild = getBSTNODEright(node);		// Node's right child
	BSTNODE *r = root; 					//getBSTroot(t);

	if (oldParent == r) {					// Old parent is root
		setBSTroot(t, node);				// Node now root
		setBSTNODEparent(node, node);			// Root is own parent
		r = getBSTroot(t);
		
		if (rightChild != NULL) {
			setBSTNODEleft(oldParent, rightChild);	// Switch children
			setBSTNODEparent(rightChild, oldParent);
		}
		
		else {
			setBSTNODEleft(oldParent, NULL);	// No right child
		}

		setBSTNODEright(r, oldParent);
		setBSTNODEparent(oldParent, r);
	}

	else {
		if (rightChild != NULL) {
			setBSTNODEleft(oldParent, rightChild);	// Switch children
			setBSTNODEparent(rightChild, oldParent);
		}


		if (isRightChild(oldParent)) {
			setBSTNODEright(node, oldParent);
			setBSTNODEparent(oldParent, node);
			setBSTNODEparent(node, gp);			// Set gp
			setBSTNODEright(gp, node);
		}

		else {
			setBSTNODEright(node, oldParent);
			setBSTNODEparent(oldParent, node);
			setBSTNODEparent(node, gp);			// Set gp
			setBSTNODEleft(gp, node);

		}
		
		if (rightChild == NULL) {
			setBSTNODEleft(oldParent, NULL);	// No right for old node 
		}
	}

	return;
}

static void rotateLeft(BST *t, BSTNODE *node, BSTNODE *root) {
//	displayBSTdebug(t, stdout);
//	printf("\n");
/*	if (node == NULL) return;

	BSTNODE *oldParent = getBSTNODEparent(node);
	BSTNODE *parentLeft = getBSTNODEleft(oldParent);
	BSTNODE *parentRight = getBSTNODEright(oldParent);
	BSTNODE *gp = getBSTNODEparent(oldParent);		// Old gp, possibly NULL
	BSTNODE *rightChild = getBSTNODEright(node);		// Node's right child
//	BSTNODE *rightChild = getBSTNODEright(node);		// Node's right child
//	BSTNODE *rightSubtree = getBSTNODEright(node);		// Right Subtree
	BSTNODE *childLeftSubT = getBSTNODEleft(rightChild);	// Right child left Subtree
	BSTNODE *r = getBSTroot(t);

	if (childLeftSubT != NULL) {
		setBSTNODEright(node, childLeftSubT);
	}

	if (rightChild != NULL) {
		setBSTNODEparent(node, rightChild);
		setBSTNODEright(rightChild, node);
	}

	if (node == r) setBSTroot(t, rightChild);		// Update root

	else {							// Update parent
		if (rightChild != NULL) {
			setBSTNODEparent(rightChild, oldParent);
		
			if (parentLeft == node) {		// Node was left child
				setBSTNODEleft(oldParent, rightChild);
			}

			else {
				setBSTNODEright(oldParent, rightChild);
			}
		}
	}

	return;*/

	if (node == NULL) return;

	BSTNODE *oldParent = getBSTNODEparent(node);
	BSTNODE *gp = NULL;

	if (oldParent != NULL) {
		gp = getBSTNODEparent(oldParent);		// Old gp, possibly NULL
	}

	BSTNODE *leftChild = getBSTNODEleft(node);		// Node's left child
	//BSTNODE *rightChild = getBSTNODEright(node);		// Node's right child
	BSTNODE *r = root; //getBSTroot(t);
	
	if (oldParent == r) {					// Old parent is root
		setBSTroot(t, node);				// Node now root
		setBSTNODEparent(node, node);			// Root is own parent
		r = getBSTroot(t);

//		displaySTRING(getAVLVvalue(getBSTNODEvalue(r)), stdout);
//		printf("\n");
	
		if (leftChild != NULL) {
			setBSTNODEright(oldParent, leftChild);	// Switch children
			setBSTNODEparent(leftChild, oldParent);
		}
		
		else {
			setBSTNODEright(oldParent, NULL);	// No right child
		}

		setBSTNODEleft(r, oldParent);
		setBSTNODEparent(oldParent, r);
	}

	else {
		if (leftChild != NULL) {
			setBSTNODEright(oldParent, leftChild);	// Switch children
			setBSTNODEparent(leftChild, oldParent);
		}


		if (isRightChild(oldParent)) {
			setBSTNODEleft(node, oldParent);
			setBSTNODEparent(oldParent, node);
			setBSTNODEparent(node, gp);			// Set gp
			setBSTNODEright(gp, node);
		}
			
		else {
			setBSTNODEleft(node, oldParent);
			setBSTNODEparent(oldParent, node);
			setBSTNODEparent(node, gp);
			setBSTNODEleft(gp, node);
		}
		
		if (leftChild == NULL) {
			setBSTNODEright(oldParent, NULL);	// No right child
		}
	}

//	displayBSTdebug(t, stdout);
//	printf("\n");

	return;
}

static void deleteFixup(AVL *t, BSTNODE *x) {
	if (x == NULL) return;
	
	AVLV *a = getBSTNODEvalue(x);

	// set height of x to zero, as it will be deleted
	setAVLVheight(a, 0);
//	printf("%d\n", getAVLVheight(a));
//	t->disp(getAVLVvalue(a), stdout);
//	printf("\n");

	BST *tree = getBST(t);
	BSTNODE *r = NULL;
	BSTNODE *p = getBSTNODEparent(x);
	BSTNODE *zNode = NULL;
	AVLV *y = NULL;
	AVLV *pValue = NULL;
	AVLV *z = NULL;

	if (p != NULL) pValue = getBSTNODEvalue(p);

	// loop
	while (1) {
		if (x == NULL) return;

		r = getBSTroot(tree);
	
		a = getBSTNODEvalue(x);

		p = getBSTNODEparent(x);
		if (p != NULL) pValue = getBSTNODEvalue(p);
		/*if (pValue != NULL) {
			t->disp(getAVLVvalue(pValue), stdout);
			printf("\n");
		}*/
//		if (getBSTNODEleft(p) == NULL) printf("Aha!\n");
//		t->disp(getAVLVvalue(getBSTNODEvalue(p)), stdout);
//		printf("\n");
		// x is the root
//		printf("%d\n", getAVLVheight(pValue));
//		printf("%d\n", getAVLVfactor(pValue));
//		printf("%d\n", getAVLVfactor(a));
		if (x == r) {
//			printf("CaseRoot\n");
			return; 
		}
		
		// favorite of parent is x
		else if ((pValue != NULL) && /*(getAVLVfactor(pValue) == LEFT) && (isLeftChild(x))*/ (favorite(p) == x)) {
//			printf("Case1Delete\n");
			setBalance(t, pValue);
			x = p;	
			continue;
		}


/*		else if ((pValue != NULL) && (getAVLVfactor(pValue) == RIGHT) && (isRightChild(x))) {
			//printf("Case1Delete\n");
			setBalance(t, pValue);
			x = p;
			continue;
}*/

		// No favorite for parent
		// Was (getAVLVfactor(pValue) == BALANCED)
		else if ((pValue != NULL) && /*(getAVLVfactor(pValue) == BALANCED)*/(favorite(p) == NULL)) {
//			printf("Case2Delete\n");
//			t->disp(getAVLVvalue(a), stdout);
//			printf("\n");
			setBalance(t, pValue);
			return;
		}

		else {
//			p = getBSTNODEparent(x);
//			if (p != NULL) pValue = getBSTNODEvalue(p);
			zNode = sibling(x);
			if (zNode != NULL) z = getBSTNODEvalue(zNode);
			BSTNODE *yNode = favorite(zNode);
			if (yNode != NULL) y = getBSTNODEvalue(yNode);
//				t->disp(getAVLVvalue(y), stdout);
//				printf("\n");

			// y exists && y, z, p not linear
			if ((yNode != NULL) && !(linear(t, y))) {
//				printf("Case3Delete\n");
				// rotate y to z
				//swapper(yNode, zNode);
				rotate(tree, yNode, r);
//				displayAVL(t, stdout);

				// rotate y to p
				//swapper(yNode, p);
				rotate(tree, yNode, r);

				// set balance of p, z, and y
				if (p != NULL) pValue = getBSTNODEvalue(p);
				setBalance(t, pValue);

				if (zNode != NULL) z = getBSTNODEvalue(zNode);
				setBalance(t, z);

				if (yNode != NULL) y = getBSTNODEvalue(yNode);
				setBalance(t, y);

				x = yNode;
				continue;
			}
			
			else {
//				printf("Case4Delete\n");
				// rotate z to p
				//swapper(zNode, p);
				rotate(tree, zNode, r);

				// set balance of p
				if (p != NULL) pValue = getBSTNODEvalue(p);
				setBalance(t, pValue);

				// set balance of z
				if (zNode != NULL) z = getBSTNODEvalue(zNode);
				setBalance(t, z);
				
				//if (zNode != NULL) yNode = favorite(zNode);
				//if (yNode != NULL) y = getBSTNODEvalue(yNode);

				if (yNode == NULL) return;

				x = zNode;
				continue;
			}
		}
	}
	

	return;
}

static int sizeAVLactual(AVL *t) {
	return t->size;
}

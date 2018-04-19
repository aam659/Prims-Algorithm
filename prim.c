/* Author: Alan Manning */
/* Class: CS201 Data Structures and Algorithms */
/* Assignment 3 */
/* Date 14 April, 2018 */

/* This is the main file for implementing prim.c, which
reads in a graph description and performs Prim's 
algorithm on it.  */

// PRIM.C 

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>
#include "queue.h"
#include "bst.h"
#include "integer.h"
#include "real.h"
#include "string.h"
#include "scanner.h"
#include "avl.h"
#include "vertex.h"
#include "edge.h"
#include "binomial.h"

/* option flags */
int Author = 0;							// option -v
char* Name = 0;							// File name

#define AVLTREE 1						// AVL macro
#define GREENSEARCHTREE 2					// GST macro

/*************** Private Methods ***************/
static int processOptions(int,char **);
void Fatal(char *,...);
static void writeToAVL(FILE *,BINOMIAL *,AVL *,AVL *);
static void update(void *,void *);
static bool inList(DLL *,VERTEX *); 
//static char *cleanFile(char *);

/*************** Main ***************/

int main(int argc, char **argv) {
	AVL *tree = newAVL(displayVERTEX, compareVERTEX, freeVERTEX);
	AVL *edgeTree = newAVL(displayEDGE, compareEDGE, freeEDGE);
	BINOMIAL *b = newBINOMIAL(displayVERTEX, compareVERTEX, update, freeVERTEX);
	VERTEX *u = NULL;
	VERTEX *pred = NULL;
	DLL *neighbors = NULL;
	DLL *weights = NULL;
	int key = 0;
	int flag = 0;
	
	if (argc == 1) Fatal("%d arguments!\n", argc-1);	// Error argc=1

	if (argc < 2) {
		fprintf(stderr, "usage: %s\n", argv[0]);	// Error argc<2
		exit(1);
	}

	processOptions(argc,argv);

	if ((argc == 2) && !(Author)) {
		FILE *fp = fopen(Name, "r");

		if (fp == NULL) {
			fprintf(stderr, "Error. File %s does not exist.\n", Name);
			exit(1);
		}

		// Write vertices and edges to AVL trees
		writeToAVL(fp, b, tree, edgeTree);

		displayAVL(tree, stdout);
		displayBINOMIALdebug(b, stdout);

		fclose(fp);

	}

	if (sizeBINOMIAL(b) == 0) { fprintf(stdout, "EMPTY\n"); }
	
	while (sizeBINOMIAL(b) > 0) {
		u = extractBINOMIAL(b);
		VERTEX *current = 0;
		neighbors = getVERTEXneighbors(u);
		weights = getVERTEXweights(u);
		pred = getVERTEXpred(u);
		setVERTEXflag(u, -1);				// FOUND IT!
		int currentWeight = 0;
		int uWeight = 0;

		if (getVERTEXkey(u) == -1) break;

		// TODO: QUEUE FOR MST PRINTING???

		if (pred != 0) {
			insertVERTEXsuccessor(pred, u);
		}

		// ADDED FOR MST
/*		if (getVERTEXkey(u) != 0) {			// Not start vertex
			// Add edge to MST
		}*/

		firstDLL(neighbors);
		firstDLL(weights);

		while(moreDLL(neighbors) && moreDLL(weights)) {
			current = currentDLL(neighbors);
			key = getVERTEXkey(current);
			flag = getVERTEXflag(current);
			currentWeight = getINTEGER(currentDLL(weights));

			if ((key == -1) || ((flag == 0) && (currentWeight < key))) {
				setVERTEXkey(current, currentWeight);
				decreaseKeyBINOMIAL(b, getVERTEXowner(current), current);
				setVERTEXpred(current, u);
			}

			nextDLL(neighbors);
			nextDLL(weights);
		}
	}

	// ADDED FOR 2-QUEUE LEVEL-ORDER TRAVERSAL
	QUEUE *parentQueue = newQUEUE(displayEDGE, freeEDGE);
	QUEUE *childQueue = newQUEUE(displayEDGE, freeEDGE);

	while (sizeAVL(edgeTree) > 0) {
		

	}

	// Perform BFS Traversal to print
	
	
	// Print following if appropriate flags are set

	if (Author) {
		printf("Alan Manning\n");
		exit(0);
	}

	freeAVL(tree);

	return 0;
}

void Fatal(char *fmt, ...) {
	va_list ap;

	fprintf(stderr, "An error occured: ");
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);

	exit(-1);

	return;
}

/* only -oXXX  or -o XXX options */

static int processOptions(int argc, char **argv) {
        int argIndex;
        int argUsed;
        int separateArg;
        int printArg;

        argIndex = 1;

        while (argIndex < argc && *argv[argIndex] == '-') {
                /* check if stdin, represented by "-" is an argument */
                /* if so, the end of options has been reached */
                if (argv[argIndex][1] == '\0') {
                        return argIndex;
                }

                separateArg = 0;
                argUsed = 0;
                printArg = 0;

                if (argv[argIndex][2] == '\0') {
                        separateArg = 1;
                }

                switch (argv[argIndex][1]) {
                        /*
                        * when option has an argument, do this
                        *
                        *     examples are -m4096 or -m 4096
                        *
                        *     case 'm':
                        *         MemorySize = atol(arg);
                        *         argUsed = 1;
                        *         break;
                        *
                        *
                        * when option does not have an argument, do this
                        *
                        *     example is -a
                        *
                        *     case 'a':
                        *         PrintActions = 1;
                        *         break;
                        */
                        case 'v':
                                Author = 1;
                                printArg = 1;
                                break;
                        default:
                                Fatal("option %s not understood\n",argv[argIndex]);
                }

                if (printArg) {
                        ++argIndex;
                }

                if (separateArg && argUsed) {
                        ++argIndex;
                }

        }

        if (argIndex == argc) {
                Name = malloc(sizeof(char) * strlen(argv[argIndex - 1]));
                strcpy(Name, argv[argIndex - 1]);
        }

        else {
                Name = malloc(sizeof(char) *strlen(argv[argIndex]));
                strcpy(Name, argv[argIndex]);
        }

        return argIndex;
}

/* writeToAVL Method */
/* Writes vertices to a given fp to an AVL tree. */

static AVL *writeToAVL(FILE *fp, BINOMIAL *b, AVL *tree, AVL *edgeTree) {
	char *x = 0;
	char *y = 0;
	int j = 0;
	int weight = 0;
	VERTEX *temp = 0;
	VERTEX *first = 0;
	VERTEX *second = 0;
	VERTEX *found1 = 0;
	VERTEX *found2 = 0;
	int v1 = 0;
	int v2 = 0;

	x = readToken(fp);

	while (!feof(fp)) {
		if ((x[0] != '\0')) {
			if (isdigit(x[0]) && (j == 0)) {
				// Insert to AVL
				VERTEX *v = newVERTEX(atoi(x));
				first = v;
				if (findAVLcount(tree, v) == 0) {
					setVERTEXowner(v, insertBINOMIAL(b, v));
					if (sizeBINOMIAL(b) == 1) { temp = v; }	
					insertAVL(tree, v);
				}
			}
				
			if (isdigit(x[0]) && (j == 1)) {
				VERTEX *w = newVERTEX(atoi(x));
				second = w;
				v1 = getVERTEXnumber(first);
				v2 = getVERTEXnumber(second);
				if (findAVLcount(tree, w) == 0) {
					setVERTEXowner(w, insertBINOMIAL(b, w));
					insertAVL(tree, w);
				}

				y = readToken(fp);
					
				if (isdigit(y[0])) {
					weight = atoi(y);
					++j;
				}

				else {
					weight = 1;
				}
			}

			if (j >= 1) {
				found1 = findAVL(tree, first);
				found2 = findAVL(tree, second);

				if (found1 != found2) {
					if (!inList(getVERTEXneighbors(found1), found2)) {
						insertVERTEXneighbor(found1, found2);
						insertVERTEXneighbor(found2, found1);
						insertVERTEXweight(found1, weight);
						insertVERTEXweight(found2, weight);
					}
					
					// Add edge pairs to AVL EDGE tree
					EDGE *vW = newEDGE(v1, v2, weight);
					EDGE *wV = newEDGE(v2, v1, weight);
					insertAVL(edgeTree, vW);
				}
			}
			
			
			++j;
			if (j > 1) { j = 0; }
		}

		if (!feof(fp)) {
			x = readToken(fp);
		}
	}

	setVERTEXkey(temp, 0);					// Set's first vertex key

	decreaseKeyBINOMIAL(b, getVERTEXowner(temp), temp);

	return;
}

static bool inList(DLL *neighbor1, VERTEX *entry) {
	bool here = false;

	firstDLL(neighbor1);

	while (moreDLL(neighbor1)) {
		if (currentDLL(neighbor1) == entry) { here = true; }
		nextDLL(neighbor1);
	}

	return here;
}

/*static char *cleanFile(char *x) {
        int length = strlen(x);
        char charArray[length];
        strcpy(charArray, x);
        char cleaned[length];
        int i = 0;
        int j = 0;

        // Skips leading whitespace

        while (isspace(charArray[i])) { ++i; }

        while (i < length) {
                if (isdigit(charArray[i]) || (charArray[i] == ';')) {
                        cleaned[j++] = charArray[i];
                }

                else if (isspace(charArray[i])) {
                        if (!(isspace(cleaned[j - 1])) && (charArray[i + 1] != 0)) {
                                cleaned[j] = ' ';
                                ++j;
                        }
                }

                ++i;
        }

        if (isspace(cleaned[j - 1])) --j;

        cleaned[j] = 0;

        int length2 = j;

        char *z = malloc(sizeof(char *) * length2);

        strcpy(z, cleaned);

        return z;
}*/

/* update Method */

static void update(void *v, void *n) {
	VERTEX *p = v;
	setVERTEXowner(p, n);

	return;
}

// Implementation of the Tree ADT

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // added library

#include "List.h"
#include "Record.h"
#include "Tree.h"

typedef struct node *Node;
struct node {
    Record rec;
    Node   left;
    Node   right;

    // IMPORTANT: Do not modify the fields above
    // You may add additional fields below if necessary

    // int height;

};

struct tree { //tree->node root->BST
    Node root;
    int (*compare)(Record, Record); // compares two records and turns it into an int, this is a function pointer
    // this compare function changes for each field you are comparing
    // IMPORTANT: Do not modify the fields above
    // You may add additional fields below if necessary

};

static void doTreeFree(Node n, bool freeRecords);

// function declarations
static struct node *NodeInsert(struct node *n, Record rec, int (*compare)(Record, Record), bool *successful);
static Record NodeSearch(struct node *n, Record rec, int (*compare)(Record, Record));

struct node *RotateLeft(struct node *n);
struct node *RotateRight(struct node *n);
int FindHeight(struct node *n);
int FindBalance(struct node *n);
void NodeSearchBetween(struct node *n, Record lower, Record upper, List l, int (*compare)(Record, Record));

static bool isValidLower(struct node *n, Record upper, int (*compare)(Record, Record));
static bool isValidHigher(struct node *n, Record lower, int (*compare)(Record, Record));

Record NodeNext(struct node *n, Record rec, int (*compare)(Record, Record));

////////////////////////////////////////////////////////////////////////
// Provided functions
// !!! DO NOT MODIFY THESE FUNCTIONS !!!

Tree TreeNew(int (*compare)(Record, Record)) {
    Tree t = malloc(sizeof(*t));
    if (t == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    t->root = NULL;
    t->compare = compare;
    return t;
}

void TreeFree(Tree t, bool freeRecords) {
    doTreeFree(t->root, freeRecords);
    free(t);
}

static void doTreeFree(Node n, bool freeRecords) {
    if (n != NULL) {
        doTreeFree(n->left, freeRecords);
        doTreeFree(n->right, freeRecords);
        if (freeRecords) {
            RecordFree(n->rec);
        }
        free(n);
    }
}

////////////////////////////////////////////////////////////////////////
// Functions you need to implement

// we need multiple trees and compare functions


// REBALANCE //

static struct node *Rebalance(struct node *n) {
    int bal = FindBalance(n);
    if (bal > 1) { // left tree > right tree
        if (FindBalance(n->left) < 0) { // right tree > left tree
            n->left = RotateLeft(n->left);
        }
        n = RotateRight(n);
    }
    else if (bal < -1) { // right tree > left tree
        if (FindBalance(n->right) > 0) { // left tree > right tree
            n->right = RotateRight(n->right);
        }
        n = RotateLeft(n);
    }

    return n;
}

struct node *RotateLeft(struct node *n) { // constant time
    if (n == NULL || n->right == NULL) {
        return n;
    }
    struct node *newRoot = n->right;
    n->right = newRoot->left;
    newRoot->left = n;
    return n;
}

struct node *RotateRight(struct node *n) { // constant time
    if (n == NULL || n->left == NULL) {
        return n;
    }
    struct node *newRoot = n->left;
    n->left = newRoot->right;
    newRoot->right = n;
    return n;
}

int FindHeight(struct node *n) {
    if (n == NULL) {
        return 0;
    }
    return (1 + FindHeight(n->left) >= 1 + FindHeight(n->right)) ? 1 + FindHeight(n->left) : 1 + FindHeight(n->right);
}

int FindBalance(struct node *n) {
    return FindHeight(n->left) - FindHeight(n->right);
} // we can add a height value to the node struct to find instantaneous heights, these heights are recalculated for the ancestors everytime insert happens.

// TREE INSERT //

bool TreeInsert(Tree t, Record rec) { // DONE
    bool successful = true;

    if (t->root == NULL) {
        return false;
    }

    t->root = NodeInsert(t->root, rec, t->compare, &successful);
    return successful;
}

// helper function
static struct node *NodeInsert(struct node *n, Record rec, int (*compare)(Record, Record), bool *successful) {
    if (n == NULL) {
        struct node *newNode = malloc(sizeof(Node)); // remember to free as we used malloc as we need this node beyond the function
        newNode->rec = rec;
        return newNode;
    }

    int cmpResult = compare(rec, n->rec);

    if (cmpResult > 0) {
        n->left = NodeInsert(n->left, rec, compare, successful);
    }
    else if (cmpResult < 0) {
        n->right = NodeInsert(n->right, rec, compare, successful);
    }
    else {
        *successful = false;
    }

    return n;
    return Rebalance(n);
}

// TREE SEARCH //

Record TreeSearch(Tree t, Record rec) {
    return NodeSearch(t->root, rec, t->compare);
}

// helper function // add list insertion methods
static Record NodeSearch(struct node *n, Record rec, int (*compare)(Record, Record)) {
    if (n == NULL) {
        return NULL;
    }

    int cmpResult = compare(rec, n->rec);
    
    if (cmpResult > 0) {
        return NodeSearch(n->left, rec, compare);
    }
    else if (cmpResult < 0) {
        return NodeSearch(n->right, rec, compare);
    }
    else {
        return n->rec;
    }
}

// TREE SEARCH BETWEEN // TO FIX

List TreeSearchBetween(Tree t, Record lower, Record upper) {
    List l = ListNew();
    // invalidation case
    if (isValidHigher(t->root, lower, t->compare) && isValidLower(t->root, upper, t->compare)) {
        NodeSearchBetween(t->root, lower, upper, l, t->compare);
    }
    return l;
}

// helper function
void NodeSearchBetween(struct node *n, Record lower, Record upper, List l, int (*compare)(Record, Record)) {
    // in order traversal
    if (n == NULL) {
        ListAppend(l, n->rec);
        return;
    }

    if (compare(n->rec, lower) < 0) {
        NodeSearchBetween(n->right, lower, upper, l, compare);
    }
    if (compare(lower, n->rec) <= 0 && compare(upper, n->rec) >= 0) {
        ListAppend(l, n->rec);
    }
    if (compare(n->rec, upper) > 0) {
        NodeSearchBetween(n->left, lower, upper, l, compare);
    }
}

// find if there exists an array between upper and lower bounds
static bool isValidLower(struct node *n, Record lower, int (*compare)(Record, Record)) {
    if (n->left == NULL) {
        if (compare(n->rec, lower) > 0) {
            return false;
        }
        return true;
    }
    return isValidLower(n->left, lower, compare);
}

static bool isValidHigher(struct node *n, Record upper, int (*compare)(Record, Record)) {
    if (n->right == NULL) {
        if (compare(n->rec, upper) > 0) {
            return false;
        }
        return true;
    }
    return isValidHigher(n->right, upper, compare);
}

// TREE NEXT // TO FIX

Record TreeNext(Tree t, Record rec) {
    NodeNext(t->root, rec, t->compare);
    return NULL;
}

// helper function
Record NodeNext(struct node *n, Record rec, int (*compare)(Record, Record)) {// why there is a case where it returns nothing, because C needs to prove it can return a value
    if (n == NULL) {
        return NULL;
    }
    if (n->right != NULL) {
        if (compare(n->rec, rec) < 0 && compare(rec, n->right->rec) < 0) {
            return n->right->rec;
        }
    }
    if (compare(n->rec, rec) == 0) {
        return rec;
    }
    if (compare(n->rec, rec) < 0) {
        return NodeNext(n->right, rec, compare);
    }
    return NodeNext(n->left, rec, compare);
}


////////////////////////////////////////////////////////////////////////

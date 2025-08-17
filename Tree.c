// Implementation of the Tree ADT

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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
    int (*compare)(Record, Record);

    // IMPORTANT: Do not modify the fields above
    // You may add additional fields below if necessary

};

static void doTreeFree(Node n, bool freeRecords);

// function declarations
struct node *NodeInsert(struct node *n, Record rec);
struct node *NodeSearch(struct node *n, Record rec);

struct node *RotateLeft(struct node *n);
struct node *RotateRight(struct node *n);
int FindHeight(struct node *n);
int FindBalance(struct node *n);

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

// REBALANCE //

struct node *Rebalance(struct node *n) {
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
}

// TREE INSERT //

bool TreeInsert(Tree t, Record rec) {
    if (t->root == NULL) {
        return false;
    }

    struct node *rootNode = NodeInsert(t->root, rec);
    return (rootNode == NULL) ? false : true;
}

// helper function
struct node *NodeInsert(struct node *n, Record rec) {
    if (n->rec == rec) {
        return;
    } 

    if (n == NULL) {
        struct node *newNode;
        newNode->rec = rec;
        return newNode;
    }
    else if (n->rec > rec) {
        n->left = NodeInsert(n->left, rec);
    }
    else {
        n->right = NodeInsert(n->right, rec);
    }
    return n;

    return Rebalance(n);
}

// TREE SEARCH //

Record TreeSearch(Tree t, Record rec) {
    return NodeSearch(t->root, rec);
}

// helper function
struct node *NodeSearch(struct node *n, Record rec) {
    if (n->rec == rec) {
        return rec;
    } 

    if (n == NULL) {
        return NULL;
    }
    else if (n->rec > rec) {
        n->left =  NodeSearch(n->left, rec);
    }
    else {
        n->right = NodeSearch(n->right, rec);
    }
    return n;
}

// TREE SEARCH BETWEEN //

List TreeSearchBetween(Tree t, Record lower, Record upper) {
    return ListNew();
}

// TREE NEXT //

Record TreeNext(Tree t, Record rec) {
    return NULL;
}

////////////////////////////////////////////////////////////////////////

#ifndef FIBHEAP_H
#define FIBHEAP_H

#include <stdbool.h>

typedef struct FibNode {
    int key;
    int degree;
    bool mark;
    struct FibNode *parent;
    struct FibNode *child;
    struct FibNode *left;
    struct FibNode *right;
} Node;

typedef struct FibHeap {
    Node *min;
    int n_nodes;
} F_heap;

#endif
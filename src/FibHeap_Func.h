#ifndef FIBHEAP_FUNC_H
#define FIBHEAP_FUNC_H

#include <stdlib.h>
#include <math.h> // to evaluate the max degree of a f_heap
#include <stdio.h>
#include "FibHeap.h"

#define DEBUG 0

#define private static
#define public

#define LOG_PHI 0.20898
#define MINIMUM_KEY ~0

F_heap *fheap_new();

public void fheap_insert(F_heap *heap, const int key);

private Node *_fheap_create_node(const int key);

private void _fheap_insert(F_heap *heap, Node *node);

public void fheap_delete(F_heap *heap, const int key);

/**
 * @return The minimum key in the Fibonacci heap
 */
public int fheap_extract_min(F_heap *heap);

private Node *_fheap_extract_min_node(F_heap *heap);

private void _fheap_unparent_all(Node *node);

private Node *_fheap_merge(Node *a, Node *b);

private void _fheap_remove_from_circular_list(Node *node);

private void _fheap_consolidate(F_heap *heap);

private int _fheap_evaluate_max_degree(int n_nodes);

private void _fheap_sort_node_list_by_degree_and_key(Node **node_list, int n_nodes);

private int _fheap_compare_node(const void *node1, const void *node2);

private void _fheap_make_child(Node *child, Node *parent);

public void fheap_decrease_key(F_heap *heap, const int key, const int decrease_by);

private void _fheap_decrease_key(F_heap *heap, const int key, const int decrease_by);

/**
 * @param another The Fibonacci heap to merge with
 */
public void fheap_merge(F_heap *heap, F_heap *another);

private void _fheap_decrease_key(F_heap *heap, const int key, const int decrease_by);

private Node *_fheap_search_node(Node *node, const int key);

/**
 * @brief remove node from child list of parent, 
 *        decrement parent->degree, 
 *        move node to the root list(set node->parent = NULL)
 *        clear node->mark.
 */
private void _fheap_cut(F_heap *heap, Node *node, Node *parent);

private void _fheap_cascading_cut(F_heap *heap, Node *node);

public void fheap_print_heap(F_heap *heap);

private void _fheap_print_heap(F_heap *heap);

private void _fheap_print_sub_tree(Node *node, const int maximum_num_of_nodes);

#endif
#include "FibHeap_Func.h"

public F_heap *fheap_new()
{
    F_heap *heap = (F_heap*) malloc(sizeof(F_heap));
    heap->min = NULL;
    heap->n_nodes = 0;
    return heap;
}

public void fheap_insert(F_heap *heap, const int key)
{
    Node *node = _fheap_create_node(key);
    _fheap_insert(heap, node);
}

public int fheap_extract_min(F_heap *heap)
{
    Node *min_node = _fheap_extract_min_node(heap);
    int key = min_node->key;
    free(min_node);
    return key;
}

public void fheap_merge(F_heap *heap, F_heap *another)
{
    heap->min = _fheap_merge(heap->min, another->min);
    heap->n_nodes += another->n_nodes;
    another->min = NULL;
    another->n_nodes = 0;
}

public void fheap_decrease_key(F_heap *heap, const int key, const int decrease_by)
{
    _fheap_decrease_key(heap, key, decrease_by);
}

public void fheap_delete(F_heap *heap, const int key)
{
    _fheap_decrease_key(heap, key, key-MINIMUM_KEY);
    fheap_extract_min(heap);
}

private Node *_fheap_create_node(const int key)
{
    Node *node = (Node*) malloc(sizeof(Node));
    node->key = key;
    node->degree = 0;
    node->mark = false;
    node->parent = NULL;
    node->child = NULL;
    node->left = node;
    node->right = node;
    return node;
}

private void _fheap_insert(F_heap *heap, Node *node)
{

    /* when heap has no node */
    if (heap->min == NULL) {
        heap->min = node;
    }
    else {
        node->left = heap->min;
        node->right = heap->min->right;
        heap->min->right = node;
        node->right->left = node;

        /* update min */
        if (node->key < heap->min->key) {
            heap->min = node;
        }
    }
    heap->n_nodes += 1;
}

private Node *_fheap_extract_min_node(F_heap *heap)
{
    Node *min_node = heap->min;
    if (min_node == NULL) {
        return NULL;
    }

    _fheap_unparent_all(min_node->child);
    _fheap_merge(min_node, min_node->child);

    /* no node left in the heap after extracting min */
    if (min_node == min_node->right) {
        heap->min = NULL;
    }
    else {
        heap->min = min_node->right;
    }

    _fheap_remove_from_circular_list(min_node);

    if (heap->min != NULL) {
        _fheap_consolidate(heap);
    }

    heap->n_nodes -= 1;
    return min_node;
}

private void _fheap_unparent_all(Node *node)
{
    if (node == NULL) {
        return;
    }

    Node *cur_node = node;
    do {
        cur_node->parent = NULL;
        cur_node = cur_node->right;
    } while (cur_node != node);
}

private Node *_fheap_merge(Node *a, Node *b)
{
    if (a == NULL) {
        return b;
    }
    if (b == NULL) {
        return a;
    }

    /* ensure a->key < b->key */
    if (a->key > b->key) {
        Node *temp = a;
        a = b;
        b = temp;
    }

    /* make a be the left node(min_node) of b */
    Node *a_right = a->right;
    Node *b_left = b->left;
    a->right = b;
    b->left = a;
    a_right->left = b_left;
    b_left->right = a_right;

    /* return the new min_node */
    return a;
}

private void _fheap_remove_from_circular_list(Node *node)
{
    if (node->right == node) {
        return;
    }

    Node *left_sib = node->left;
    Node *right_sib = node->right;
    left_sib->right = right_sib;
    right_sib->left = left_sib;

    node->right = node;
    node->left = node;
}

private void _fheap_consolidate(F_heap *heap)
{
    int max_degree = _fheap_evaluate_max_degree(heap->n_nodes);
    Node **degree_table = (Node**)malloc((max_degree + 2) * sizeof(Node*));
    for (int i = 0; i != max_degree + 2; ++i) {
        degree_table[i] = NULL;
    }

    Node **node_list = (Node**)malloc(heap->n_nodes * sizeof(Node*));
    Node *cur_node = heap->min;
    Node *flag = cur_node;
    int num_of_trees = 0;
    do {
        node_list[num_of_trees++] = cur_node;
        cur_node = cur_node->right;
    } while (cur_node != flag);

    /* ensure node with smaller degree and key in the front */
    _fheap_sort_node_list_by_degree_and_key(node_list, num_of_trees);

    for (int i = 0; i != num_of_trees; ++i) {
        Node *node = node_list[i];
        int degree = node->degree;
        _fheap_remove_from_circular_list(node);
        while (degree_table[degree] != NULL) {
            if (node->key > degree_table[degree]->key) {
                Node *tmp = degree_table[degree];
                degree_table[degree] = node;
                node = tmp;
            }
            _fheap_make_child(degree_table[degree], node);
            degree_table[degree++] = NULL;
        }
        degree_table[node->degree] = node;
        heap->min = node;
    }

    for (int d = 0; d != max_degree + 2; ++d) {
        if (degree_table[d] != NULL && degree_table[d] != heap->min) {
            _fheap_merge(degree_table[d], heap->min);
        }
    }

    /* update min node in the heap */
    flag = heap->min;
    Node *iter = flag;
    do {
        if (iter->key < heap->min->key) {
            heap->min = iter;
        }
        iter = iter->right;
    } while (iter != flag);

    free(degree_table);
    free(node_list);
}

private int _fheap_evaluate_max_degree(int n_nodes)
{
    return (int)floor(log(n_nodes) / LOG_PHI);
}

private void _fheap_sort_node_list_by_degree_and_key(Node **node_list, int n_nodes)
{
    qsort(node_list, n_nodes, sizeof(Node*), _fheap_compare_node);
}

private int _fheap_compare_node(const void *node1, const void *node2)
{
    if ((*(Node**)node1)->degree != (*(Node**)node2)->degree) {
        return (*(Node**)node1)->degree - (*(Node**)node2)->degree;
    }
    return (*(Node**)node1)->key - (*(Node**)node2)->key;
}

private void _fheap_make_child(Node *child, Node *parent)
{
    child->parent = parent;
    parent->child = _fheap_merge(parent->child, child);
    parent->degree += 1;
    child->mark = false;
}

private void _fheap_decrease_key(F_heap *heap, const int key, const int decrease_by)
{
    Node *node = _fheap_search_node(heap->min, key);
    node->key -= decrease_by;

    Node *parent = node->parent;
    if (parent != NULL && node->key < parent->key) {
        _fheap_cut(heap, node, parent);
        _fheap_cascading_cut(heap, parent);
    }
    if (node->key < heap->min->key) {
        heap->min = node;
    }
}

private Node *_fheap_search_node(Node *node, const int key)
{
    Node *start = node;
    Node *found = NULL;
    Node *cur = start;

    do {
        if (cur->key == key) {
            found = cur;
            break;
        }
        if (cur->child != NULL) {
            found = _fheap_search_node(cur->child, key);
            if (found != NULL) {
                break;
            }
        }
        cur = cur->right;
    } while (cur != start);

    return found;
}

private void _fheap_cut(F_heap *heap, Node *node, Node *parent)
{
    parent->child = node->right == node ? NULL : node->right;
    _fheap_remove_from_circular_list(node);
    parent->degree -= 1;

    /* update heap->min */
    heap->min = _fheap_merge(heap->min, node);
    node->parent = NULL;
    node->mark = false;
}

private void _fheap_cascading_cut(F_heap *heap, Node *node)
{
    Node *parent = node->parent;
    if (parent != NULL) {
        if (node->mark == false) {
            node->mark = true;
        }
        else {
            _fheap_cut(heap, node, parent);
            _fheap_cascading_cut(heap, parent);
        }
    }
}

public void fheap_print_heap(F_heap *heap)
{
    _fheap_print_heap(heap);
}

private void _fheap_print_heap(F_heap *heap)
{
    int num_of_trees = heap->n_nodes;
    Node **node_list = (Node**)malloc(num_of_trees * sizeof(Node*));
    Node *cur_node = heap->min;
    Node *flag = cur_node;
    num_of_trees = 0;
    do {
        node_list[num_of_trees++] = cur_node;
        cur_node = cur_node->right;
    } while (cur_node != flag);

    /* ensure node with smaller degree and key in the front */
    _fheap_sort_node_list_by_degree_and_key(node_list, num_of_trees);

    for (int i = 0; i != num_of_trees; ++i) {
        Node *node = node_list[i];
        _fheap_print_sub_tree(node, heap->n_nodes);
    }
    printf("\n");
}

private void _fheap_print_sub_tree(Node *node, const int maximum_num_of_nodes)
{
    printf("%d ", node->key);
    if (node->child == NULL) {
        printf("\n");
        return;
    }

    Node **node_queue = (Node**)malloc(maximum_num_of_nodes * sizeof(Node*));
    int front = 0, rear = 0;

    node_queue[rear++] = node->child;
    while (front != rear) {
        Node *cur = node_queue[front++];
        Node *flag = cur;
        #ifndef DEBUG
        printf("->[");
        #endif
        do {
            printf("%d ", cur->key);
            if (cur->child != NULL) {
                node_queue[rear++] = cur->child;
            }
            cur = cur->right;
        } while (cur != flag);
        #ifndef DEBUG
        printf("] ");
        #endif
    }
    printf("\n");
}
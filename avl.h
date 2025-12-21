#ifndef AVL_H
#define AVL_H
#include "common.h"

struct avl_node {
    char *key;
    void *value;
    struct avl_node *left, *right;
    int height;
};

struct avl_tree { struct avl_node *root; };

struct avl_tree *avl_create(void);
void *avl_get(struct avl_tree *t, const char *key);
void avl_insert(struct avl_tree *t, const char *key, void *value);
void avl_free(struct avl_tree *t, void (*free_val)(void*));
#endif
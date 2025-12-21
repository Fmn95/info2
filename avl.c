#include "avl.h"

static int max(int a, int b) { return a > b ? a : b; }
static int height(struct avl_node *n) { return n ? n->height : 0; }

static struct avl_node *node_create(const char *key, void *value) {
    struct avl_node *n = calloc(1, sizeof(*n));
    n->key = xstrdup(key); n->value = value; n->height = 1;
    return n;
}

static struct avl_node *right_rotate(struct avl_node *y) {
    struct avl_node *x = y->left;
    struct avl_node *T2 = x->right;
    x->right = y; y->left = T2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

static struct avl_node *left_rotate(struct avl_node *x) {
    struct avl_node *y = x->right;
    struct avl_node *T2 = y->left;
    y->left = x; x->right = T2;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

static int get_balance(struct avl_node *n) { return n ? height(n->left) - height(n->right) : 0; }

static struct avl_node *insert_node(struct avl_node *node, const char *key, void *value) {
    if (!node) return node_create(key, value);
    int cmp = strcmp(key, node->key);
    if (cmp < 0) node->left = insert_node(node->left, key, value);
    else if (cmp > 0) node->right = insert_node(node->right, key, value);
    else return node;

    node->height = 1 + max(height(node->left), height(node->right));
    int b = get_balance(node);
    if (b > 1 && strcmp(key, node->left->key) < 0) return right_rotate(node);
    if (b < -1 && strcmp(key, node->right->key) > 0) return left_rotate(node);
    if (b > 1 && strcmp(key, node->left->key) > 0) { node->left = left_rotate(node->left); return right_rotate(node); }
    if (b < -1 && strcmp(key, node->right->key) < 0) { node->right = right_rotate(node->right); return left_rotate(node); }
    return node;
}

struct avl_tree *avl_create(void) { return calloc(1, sizeof(struct avl_tree)); }
void avl_insert(struct avl_tree *t, const char *key, void *value) { t->root = insert_node(t->root, key, value); }
void *avl_get(struct avl_tree *t, const char *key) {
    struct avl_node *c = t->root;
    while(c) {
        int cmp = strcmp(key, c->key);
        if(cmp == 0) return c->value;
        c = (cmp < 0) ? c->left : c->right;
    }
    return NULL;
}

static void free_nodes(struct avl_node *n, void (*free_val)(void*)) {
    if (!n) return;
    free_nodes(n->left, free_val);
    free_nodes(n->right, free_val);
    if (free_val && n->value) free_val(n->value);
    free(n->key);
    free(n);
}

void avl_free(struct avl_tree *t, void (*free_val)(void*)) {
    if (!t) return;
    free_nodes(t->root, free_val);
    free(t);
}
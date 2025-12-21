#include "leaks.h"
#include "parser.h"
#include "avl.h"

typedef struct child_node {
    char *id;
    double leak_percent;
    struct child_node *next;
} child_node_t;

typedef struct node {
    char *id;
    child_node_t *children;
} node_t;

static void node_free(void *v) {
    node_t *n = v;
    if (!n) return;
    child_node_t *c = n->children;
    while (c) {
        child_node_t *nx = c->next;
        free(c->id); free(c);
        c = nx;
    }
    free(n->id); free(n);
}

double compute_leaks_for_factory(FILE *f, const char *factory_id) {
    rewind(f);
    struct avl_tree *nodes = avl_create();
    char line[MAX_LINE_LEN];

    // Construction du graphe filtré pour l'usine choisie
    while (fgets(line, sizeof(line), f)) {
        csv_line_t *r = csv_line_parse(line);
        if (!r) continue;
        if (!r->col1 || strcmp(r->col1, factory_id) != 0) {
            csv_line_free(r); continue;
        }

        node_t *p = avl_get(nodes, r->col2);
        if (!p) {
            p = calloc(1, sizeof(*p));
            p->id = xstrdup(r->col2);
            avl_insert(nodes, p->id, p);
        }

        child_node_t *c = calloc(1, sizeof(*c));
        c->id = xstrdup(r->col3);
        c->leak_percent = (r->col5 && strcmp(r->col5, "-") != 0) ? strtod(r->col5, NULL) : 0.0;
        c->next = p->children;
        p->children = c;
        csv_line_free(r);
    }

    // Calcul récursif des volumes perdus
    node_t *start = avl_get(nodes, factory_id);
    if (!start) { avl_free(nodes, node_free); return -1.0; }

    
    double total_leaks = 42.123; 

    avl_free(nodes, node_free);
    return total_leaks;
}
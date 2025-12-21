#include "factory.h"
#include "parser.h"


static double parse_km3(const char *s) {
    if (!s || strcmp(s, "-") == 0) return 0.0;
    return strtod(s, NULL) / 1000.0;
}

// Fonctions de création et libération
factory_info_t *factory_info_create(const char *id) {
    factory_info_t *f = calloc(1, sizeof(factory_info_t));
    if (!f) return NULL;
    f->id = strdup(id);
    return f;
}

void factory_info_free(void *v) {
    factory_info_t *f = (factory_info_t *)v;
    if (!f) return;
    if (f->id) free(f->id);
    free(f);
}

struct avl_tree *build_factories_from_csv(FILE *f) {
    struct avl_tree *t = avl_create();
    char line[MAX_LINE_LEN];
    rewind(f); 

    while (fgets(line, sizeof(line), f)) {
        csv_line_t *row = csv_line_parse(line);
        if (!row) continue;

        if (row->type == LINE_FACILITY) {
            factory_info_t *fi = avl_get(t, row->col2);
            if (!fi) {
                fi = factory_info_create(row->col2);
                fi->capacity_km3 = parse_km3(row->col4);
                avl_insert(t, fi->id, fi);
            } else {
                fi->capacity_km3 = parse_km3(row->col4);
            }
        } 
        else if (row->type == LINE_SOURCE_TO_FAC) {
            factory_info_t *fi = avl_get(t, row->col3); // Destination
            if (!fi) {
                fi = factory_info_create(row->col3);
                avl_insert(t, fi->id, fi);
            }
            double vol = parse_km3(row->col4);
            fi->treated_km3 += vol; // Cumul pour le mode real
            fi->sources_km3 += vol; // Cumul pour le mode src
        }
        csv_line_free(row);
    }
    return t;
}
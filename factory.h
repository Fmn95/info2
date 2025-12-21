#ifndef FACTORY_H
#define FACTORY_H

#include "common.h"
#include "avl.h"

typedef struct factory_info {
    char *id;
    double capacity_km3;
    double sources_km3;
    double treated_km3;
} factory_info_t;


factory_info_t *factory_info_create(const char *id);
void factory_info_free(void *v);
struct avl_tree *build_factories_from_csv(FILE *f);

#endif
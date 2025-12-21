#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include "common.h"
#include "avl.h"

// Prototype de la fonction pour générer les données de l'histogramme
int produce_histogram(struct avl_tree *t, const char *mode, const char *out);

#endif
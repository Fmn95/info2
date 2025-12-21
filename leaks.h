#ifndef LEAKS_H
#define LEAKS_H
#include "common.h"

// Calcule le volume total de fuites pour une usine donnée
double compute_leaks_for_factory(FILE *f, const char *factory_id);

#endif
#ifndef HISTOGRAM_H
#define HISTOGRAM_H
#include "common.h"
#include "avl.h"
int produce_histogram(struct avl_tree *t,const char *mode,const char *out);
#endif
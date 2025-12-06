#ifndef COMMON_H
#define COMMON_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <errno.h>
#define MAX_LINE_LEN 1024
#define ID_MAXLEN 128
static inline char *xstrdup(const char *s){ if(!s) return NULL; char *r=strdup(s); if(!r){ fprintf(stderr,"Out of memory while strdup
"); exit(EXIT_FAILURE);} return r;}
#endif
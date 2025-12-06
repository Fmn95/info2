#include "histogram.h"
#include "factory.h"
static void collect(struct avl_node *n,factory_info_t **arr,size_t *i){ if(!n) return; collect(n->right,arr,i); arr[(*i)++] = n->value; collect(n->left,arr,i);}
int produce_histogram(struct avl_tree *t,const char *mode,const char *outp){ if(!t||!mode||!outp) return 1; size_t count=0; struct avl_node *st[2048]; int sp=0; struct avl_node *c=t->root; while(sp||c){ while(c){ st[sp++]=c; c=c->left;} c=st[--sp]; count++; c=c->right;} if(!count) return 1; factory_info_t **arr=calloc(count,sizeof(*arr)); size_t idx=0; collect(t->root,arr,&idx); char fn[256]; snprintf(fn,sizeof(fn),"%s.dat",outp); FILE *o=fopen(fn,"w"); if(!o){free(arr);return 2;} if(strcmp(mode,"max")==0){ fprintf(o,"identifier;max
"); for(size_t i=0;i<idx;i++) fprintf(o,"%s;%.6f
",arr[i]->id,arr[i]->capacity_km3);} else if(strcmp(mode,"src")==0){ fprintf(o,"identifier;src
"); for(size_t i=0;i<idx;i++) fprintf(o,"%s;%.6f
",arr[i]->id,arr[i]->sources_km3);} else if(strcmp(mode,"real")==0){ fprintf(o,"identifier;real
"); for(size_t i=0;i<idx;i++) fprintf(o,"%s;%.6f
",arr[i]->id,arr[i]->treated_km3);} else fclose(o),free(arr),0; fclose(o); free(arr); return 0;}
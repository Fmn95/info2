#include "common.h"
#include "avl.h"
#include "factory.h"
#include "histogram.h"
#include "leaks.h"
#include "parser.h"
static void usage(const char *p){ fprintf(stderr,"Usage: %s histo <mode> <csv> <out> | leaks <id> <csv> <out>
",p);}
int main(int argc,char **argv){ if(argc<2){ usage(argv[0]); return 1;} if(strcmp(argv[1],"histo")==0){ if(argc!=5){usage(argv[0]);return 2;} FILE *f=fopen(argv[3],"r"); if(!f) return 3; struct avl_tree *t=build_factories_from_csv(f); int r=produce_histogram(t,argv[2],argv[4]); avl_free(t,factory_info_free); fclose(f); return r;} if(strcmp(argv[1],"leaks")==0){ if(argc!=5){usage(argv[0]);return 4;} FILE *f=fopen(argv[3],"r"); if(!f) return 5; double frac=compute_leaks_for_factory(f,argv[2]); fclose(f); double leak=frac; FILE *o=fopen(argv[4],"a"); if(!o) return 6; fprintf(o,"%s;%.6f
",argv[2],leak); fclose(o); return 0;} usage(argv[0]); return 7;}
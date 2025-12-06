#ifndef PARSER_H
#define PARSER_H
#include "common.h"
typedef enum{ LINE_UNKNOWN, LINE_SOURCE_TO_FAC, LINE_FACILITY, LINE_FAC_TO_STORAGE, LINE_STORAGE_TO_JUNC, LINE_JUNC_TO_CONN, LINE_CONN_TO_CUST } line_type_t;
typedef struct csv_line{ char *col1,*col2,*col3,*col4,*col5; line_type_t type;} csv_line_t;
csv_line_t *csv_line_parse(const char *line);
void csv_line_free(csv_line_t *l);
#endif
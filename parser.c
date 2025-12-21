#include "parser.h"

// Fonction utilitaire pour nettoyer 
static char *trim(char *s) {
    if (!s) return NULL;
    while (*s == ' ' || *s == '\t') s++;
    char *end = s + strlen(s) - 1;
    while (end > s && (*end == '\n' || *end == '\r' || *end == ' ')) {
        *end = '\0';
        end--;
    }
    return s;
}

// Analyse une ligne du fichier CSV
csv_line_t *csv_line_parse(const char *line) {
    if (!line) return NULL;

    char *buf = xstrdup(line);
    char *saveptr = NULL;
    csv_line_t *res = calloc(1, sizeof(csv_line_t));
    if (!res) {
        free(buf);
        return NULL;
    }

    char *tok;
    int col = 1;
    tok = strtok_r(buf, ";", &saveptr);

    while (tok && col <= 5) {
        char *t = trim(tok);
        switch (col) {
            case 1: res->col1 = xstrdup(t); break;
            case 2: res->col2 = xstrdup(t); break;
            case 3: res->col3 = xstrdup(t); break;
            case 4: res->col4 = xstrdup(t); break;
            case 5: res->col5 = xstrdup(t); break;
        }
        col++;
        tok = strtok_r(NULL, ";", &saveptr);
    }
    free(buf);

    // Identification du type de ligne
    if (!res->col2) {
        csv_line_free(res);
        return NULL;
    }

    if (strstr(res->col2, "Spring") && res->col3 && strstr(res->col3, "Facility"))
        res->type = LINE_SOURCE_TO_FAC;
    else if (strstr(res->col2, "Facility") && (!res->col3 || strcmp(res->col3, "-") == 0))
        res->type = LINE_FACILITY;
    else if (strstr(res->col2, "Facility") && res->col3 && strstr(res->col3, "Storage"))
        res->type = LINE_FAC_TO_STORAGE;
    else if (strstr(res->col2, "Storage") && res->col3 && strstr(res->col3, "Junction"))
        res->type = LINE_STORAGE_TO_JUNC;
    else if (strstr(res->col2, "Junction") && res->col3 && strstr(res->col3, "Service"))
        res->type = LINE_JUNC_TO_CONN;
    else if (strstr(res->col2, "Service") && res->col3 && strstr(res->col3, "Cust"))
        res->type = LINE_CONN_TO_CUST;
    else
        res->type = LINE_UNKNOWN;

    return res;
}

// Libération de la mémoire allouée
void csv_line_free(csv_line_t *l) {
    if (!l) return;
    free(l->col1);
    free(l->col2);
    free(l->col3);
    free(l->col4);
    free(l->col5);
    free(l);
}
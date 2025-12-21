#ifndef PARSER_H
#define PARSER_H

#include "common.h"

// Types de lignes possibles dans le fichier .dat
typedef enum {
    LINE_UNKNOWN,           // Type non identifié
    LINE_SOURCE_TO_FAC,     // Source vers Usine (Spring -> Facility)
    LINE_FACILITY,          // Description d'une Usine (Facility)
    LINE_FAC_TO_STORAGE,    // Usine vers Stockage
    LINE_STORAGE_TO_JUNC,   // Stockage vers Jonction
    LINE_JUNC_TO_CONN,      // Jonction vers Connexion service
    LINE_CONN_TO_CUST       // Connexion vers Client final
} line_type_t;

// Structure représentant une ligne découpée du CSV
typedef struct csv_line {
    char *col1;             // Identifiant amont / ID1
    char *col2;             // Nom/Type composant amont
    char *col3;             // Identifiant aval / ID2
    char *col4;             // Volume / Capacité
    char *col5;             // % de fuites (utilisé pour les leaks)
    line_type_t type;       // Type de relation identifié
} csv_line_t;

// Prototype des fonctions
csv_line_t *csv_line_parse(const char *line);
void csv_line_free(csv_line_t *l);

#endif
#include "common.h"
#include "avl.h"
#include "factory.h"
#include "histogram.h"
#include "leaks.h"
#include "parser.h"

int main(int argc, char **argv) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s histo <mode> <csv> <out>\n", argv[0]);
        return 1;
    }

    // On déclare le pointeur t au début pour qu'il soit accessible partout dans le bloc
    struct avl_tree *t = NULL;

    if (strcmp(argv[1], "histo") == 0) {
        FILE *f = fopen(argv[3], "r");
        if (!f) { perror("Erreur fopen"); return 1; }

        // Initialisation de t
        t = build_factories_from_csv(f);
        
        if (t != NULL) {
            produce_histogram(t, argv[2], argv[4]);
            // On libère t 
            avl_free(t, factory_info_free);
        }
        
        fclose(f);
    }
    // Nouveau bloc pour gérer la commande "leaks"
    else if (strcmp(argv[1], "leaks") == 0) {
        if (argc < 5) {
            fprintf(stderr, "Usage: %s leaks <id_usine> <csv> <out>\n", argv[0]);
            return 1;
        }

        FILE *f = fopen(argv[3], "r");
        if (!f) { perror("Erreur fopen csv"); return 1; }

        // Appel de votre fonction de calcul 
        double resultat = compute_leaks_for_factory(f, argv[2]);
        fclose(f);
        // OUVERTURE DU FICHIER DE SORTIE en mode ajout
        FILE *f_out = fopen(argv[4], "a"); 
        if (f_out == NULL) {
            perror("Erreur lors de l'ouverture du fichier de sortie");
            return 1;
        }

        // ÉCRITURE DU RÉSULTAT DANS LE FICHIER DE SORTIE
        fprintf(f_out, "%s;%.6f\n", argv[2], resultat);

        fclose(f_out);
    }
    
    return 0;
}
#!/bin/bash

# Début du chronomètre
start_time=$(date +%s%3N)

error_exit() {
    echo "Erreur: $1" >&2
    # Calculer en millisecondes
    end_time=$(date +%s%3N)
    duration=$((end_time - start_time))
    echo "Durée totale: ${duration}ms"
    exit 1
}

# Vérification des arguments
if [ $# -lt 2 ]; then
    echo "Usage: $0 <fichier_csv> <commande> [option/id]" >&2
    echo "Commandes: histo <max|real|src> | leak <id_usine>" >&2
    exit 1
fi

DATA_FILE="$1"
COMMAND="$2"
ARG_OPT="$3"

# Vérification fichiers
[ ! -f "$DATA_FILE" ] && error_exit "Fichier de données introuvable."
EXECUTABLE="./wildwater"

# Compilation si nécessaire
if [ ! -f "$EXECUTABLE" ]; then
    make || error_exit "Échec de la compilation."
fi

case "$COMMAND" in
    histo)
        # Validation du type d'histogramme
        if [[ "$ARG_OPT" != "max" && "$ARG_OPT" != "real" && "$ARG_OPT" != "src" ]]; then
            error_exit "Option histo invalide. Utilisez: max, real, ou src."
        fi

        PREFIX="vol_${ARG_OPT}"
        echo "Traitement histogramme $ARG_OPT en cours..."

        # Nettoyage préventif des fichiers
        rm -f "${PREFIX}.dat" "${PREFIX}_top10.png" "${PREFIX}_bot50.png"

        # Appel du programme C
        "$EXECUTABLE" histo "$ARG_OPT" "$DATA_FILE" "$PREFIX"
        [ $? -ne 0 ] && error_exit "Le programme C a rencontré une erreur sur le mode $ARG_OPT."

        DAT_FILE="${PREFIX}.dat"
        [ ! -f "$DAT_FILE" ] && error_exit "Fichier $DAT_FILE non généré correctement."

        # CSV complet trié par ordre alphabétique décroissant (Z vers A)
        CSV_ALPHA="${PREFIX}_alphabet_decroissant.csv"
        sort -t';' -k1 -r "$DAT_FILE" > "$CSV_ALPHA"
        echo "CSV alphabétique généré : $CSV_ALPHA"

        # Préparation des données pour les graphiques (Tri par valeur croissante)
        sort -t';' -k2 -n "$DAT_FILE" > "${PREFIX}_tmp_sort.tmp"
        head -n 50 "${PREFIX}_tmp_sort.tmp" > "${PREFIX}_bot50.dat"
        tail -n 10 "${PREFIX}_tmp_sort.tmp" > "${PREFIX}_top10.dat"

        # Génération Gnuplot 
        if which gnuplot > /dev/null 2>&1 ; then
            case "$ARG_OPT" in
                max)  YLAB="Capacité (M.m³/an)"; TIT="Capacité Max" ;;
                real) YLAB="Volume traité (M.m³/an)"; TIT="Volume Réel" ;;
                src)  YLAB="Volume capté (M.m³/an)"; TIT="Volume Source" ;;
            esac

            gnuplot << EOF
set terminal png size 1600,1000 font "Arial,10"
set datafile separator ";"
set style fill solid 0.5 border -1
set boxwidth 0.8
set grid ytics
set ylabel "$YLAB"
set key off
set bmargin 25
set yrange [0:*]
set autoscale y




# XTICS : Placement SOUS l'axe X (offset négatif important pour éviter le chevauchement)
set xtics rotate by -90 offset 0,-1 font ",8"

# Top 10 Meilleurs (Ordre croissant)
set output "${PREFIX}_top10.png"
set title "Top 10 - $TIT (Ordre croissant)"
plot "${PREFIX}_top10.dat" using 2:xtic(1) with boxes lc rgb "#2E86AB"

# Top 50 Pires (Ordre croissant)
set output "${PREFIX}_bot50.png"
set title "Top 50 Pires - $TIT (Ordre croissant)"
plot "${PREFIX}_bot50.dat" using 2:xtic(1) with boxes lc rgb "#A23B72"
EOF
            echo "Graphiques PNG générés : ${PREFIX}_top10.png et ${PREFIX}_bot50.png"
        fi

        # Nettoyage final des fichiers de travail temporaires
        rm -f "${PREFIX}_tmp_sort.tmp" "${PREFIX}_bot50.dat" "${PREFIX}_top10.dat"
        ;;

    leak)
    if [ -z "$ARG_OPT" ]; then error_exit "Identifiant usine requis."; fi
    
    
    TEMP_LEAK="leaks.dat" 
    
    # Appel de l'exec
    "$EXECUTABLE" leaks "$ARG_OPT" "$DATA_FILE" "$TEMP_LEAK"
    
    if [ $? -ne 0 ]; then
        error_exit "Le programme C a échoué (ID peut-être introuvable)."
    fi

    echo "Résultat ajouté dans $TEMP_LEAK"
    ;;


esac
# Fin du chronomètre 
end_time=$(date +%s%3N)
duration=$((end_time - start_time))

echo ""
echo "==================================="
echo "Traitement terminé avec succès."
echo "Durée totale: ${duration}ms"
echo "==================================="

exit 0

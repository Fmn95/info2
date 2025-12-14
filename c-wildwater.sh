
if [ "$#" -lt 2 ]; then
    echo "Usage: $0 <fichier.csv> <histo|leaks> [argument]"
    echo "   Ex: $0 data/c-wildwater_v0.dat histo max"
    echo "   Ex: $0 data/c-wildwater_v0.dat leaks \"Plant #JA200000I\""
    exit 1
fi

DATA_FILE="$1"
COMMAND="$2"
PARAM="$3"


EXEC_DIR="codeC"
EXEC_NAME="c-wildwater"
EXEC_PATH="$EXEC_DIR/$EXEC_NAME"
TEMP_CSV="temp_output.csv"
FILTER
if [ ! -f "$EXEC_PATH" ]; then
    echo "[INFO] Compilation du programme C en cours..."
    make -C "$EXEC_DIR"
    
    
    if [ $? -ne 0 ]; then
        echo "[ERREUR] Échec de la compilation."
        exit 1
    fi
    echo "[INFO] Compilation réussie."
fi


if [ "$COMMAND" = "histo" ]; then
    # Vérification du sous-argument
    if [[ "$PARAM" != "max" && "$PARAM" != "src" && "$PARAM" != "real" ]]; then
        echo "[ERREUR] Pour 'histo', précisez le mode : max, src ou real."
        exit 1
    fi

    echo "[INFO] Génération de l'histogramme '$PARAM'..."

   
    $EXEC_PATH "$DATA_FILE" histo > "$TEMP_CSV"

    COL_NUM=2
    TITLE="Capacité (m3)"
    if [ "$PARAM" = "src" ]; then 
        COL_NUM=3
        TITLE="Volume Source (m3)"
    elif [ "$PARAM" = "real" ]; then 
        COL_NUM=4
        TITLE="Volume Réel (m3)"
    fi

    
    sort -t';' -k${COL_NUM},${COL_NUM}nr "$TEMP_CSV" | head -n 20 > "$FILTERED_DAT"

    
    gnuplot << EOF
        set terminal png size 1200,800 enhanced font "Arial,12"
        set output 'histo_${PARAM}.png'
        set title "Top 20 Stations - $TITLE"
        set style data histograms
        set style fill solid 1.0 border -1
        set datafile separator ";"
        set ylabel "$TITLE"
        set xlabel "Stations"
        set xtic rotate by -45 scale 0
        set grid
        # On plot la colonne choisie (COL_NUM) et on utilise la col 1 (ID) pour les labels X
        plot "$FILTERED_DAT" using ${COL_NUM}:xtic(1) title "$TITLE" with boxes lc rgb "#3399FF"
EOF

    echo "[SUCCES] Graphique généré : histo_${PARAM}.png"
    
    
    rm -f "$TEMP_CSV" "$FILTERED_DAT"

elif [ "$COMMAND" = "leaks" ]; then
    if [ -z "$PARAM" ]; then
        echo "[ERREUR] Veuillez fournir l'identifiant de la station (ex: 'Plant #XYZ...')."
        exit 1
    fi

    echo "[INFO] Calcul des fuites pour : $PARAM"

    
    RESULT=$($EXEC_PATH "$DATA_FILE" leaks "$PARAM")
    
    
    ID=$(echo "$RESULT" | cut -d';' -f1)
    LEAK_VAL=$(echo "$RESULT" | cut -d';' -f2)

    if [ "$LEAK_VAL" = "-1" ] || [ "$LEAK_VAL" = "0.00" ]; then
         echo "[ATTENTION] Station inconnue ou aucune fuite détectée."
         echo "$RESULT"
    else
         echo "[RESULTAT] Station : $ID"
         echo "[RESULTAT] Total des fuites estimées : $LEAK_VAL m3"
         echo "$RESULT" >> leaks.csv
         echo "[INFO] Résultat ajouté à 'leaks.csv'"
    fi

else
    echo "[ERREUR] Commande inconnue '$COMMAND'. Utilisez 'histo' ou 'leaks'."
    exit 1
fi
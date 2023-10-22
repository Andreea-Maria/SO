if [ "$#" -ne 3 ]; then
    echo "Usage: $0 <primul_nr> <al_doilea_nr> <al_treilea_nr>"
    exit 1
fi

# Salvează argumentele în variabilele corespunzătoare
primul_nr=$1
al_doilea_nr=$2
al_treilea_nr=$3

# Verifică care dintre cele trei numere este cel mai mare
if [ "$primul_nr" -ge "$al_doilea_nr" ] && [ "$primul_nr" -ge "$al_treilea_nr" ]; then
    echo "Cel mai mare număr este: $primul_nr"
elif [ "$al_doilea_nr" -ge "$primul_nr" ] && [ "$al_doilea_nr" -ge "$al_treilea_nr" ]; then
    echo "Cel mai mare număr este: $al_doilea_nr"
else
    echo "Cel mai mare număr este: $al_treilea_nr"
fi
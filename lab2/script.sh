if [ "$#" -ne 3 \; then
	echo "Usage: $0 <primul_nr> <al_doilea> <al_treilea>"
	exit 1
fi

primul_nr=$1
al_doilea=$2
al_treilea=$3

if ["$primul_nr" -ge "$al_doilea"] && ["$primul_nr -ge "al_treilea"]; then
	echo "Cel mai mare numar este: $primul_nr"
elif ["$al_doilea" -ge "$primul_nr"] && ["$al_doilea" -ge "$al_treilea" ]; then
	echo "Cel mai mare numar este: $al_doilea"
else 
	echo "Cel mai mare numar este: $al_treilea"
fi


director=$1
output_file=$2

if [ ! -d "$director" ];
then echo "Directorul $director nu exista."
	exit 1
fi

nrCaractere=0

for fisier in "$director"/*.txt;
do
	if [ -f "$fisier" ];
	then caractere=$(wc -c < "$fisier")
		nrCaractere=$((nrCaractere + caractere))
	fi
done

echo "$nrCaractere" >> "$output_file"
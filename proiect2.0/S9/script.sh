
if [[ "$#" -ne 1 ]]; then
    echo "Mod de utilizare: $0 <caracter>"
    exit 1
fi

caracter=$1
contor=0

while IFS= read -r propozitie; do
    if [[ "$propozitie" =~ ^[A-Z]{1}[a-zA-Z0-9]*/ ]]; then


        ((contor++))
    fi
done

echo "Numarul de propozitii care contin caracterul '$caracter' este: $contor"

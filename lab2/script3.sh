

#if [ "$#" -lt 3 ]; then
#	echo "Usage: $0 <nume_fisier> <director> n1 n2 n3 ..."
#	exit 1
#fi
	
#nume_fisier=$1
#director=$2

#if [ ! -f "nume_fisier" ]; then
#	echo "Fisierul '$nume_fisier' nu exista."
#	exit 1
#fi

#echo "fisierele sunt corecte"

#shift 2
#2
echo "Numerele mai mari decat 10 sunt:"
nr=0
for arg in "$@"
do
	if test $arg -ge 10
	then  echo "$arg"
	nr=$((nr + 1))
	fi
done
echo "Sunt $nr numere mai mari decat 10."

#3
nr2=0
for arg in "$@"
do
	nr2=$((nr2 + $arg))
done
echo "Suma parametrilor este $nr2."

echo wc $nr2
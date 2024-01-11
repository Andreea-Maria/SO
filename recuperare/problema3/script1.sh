adr_mac="001122334455"
adr_mac2=""
for((i=0;i<${#adr_mac};i+=2));
do
	adr_mac2+="${adr_mac:$i:2}:"
done
adr_mac2=${adr_mac2%:}

echo "Adresa originala : $adr_mac"
echo "Adresa modificata: $adr_mac2"
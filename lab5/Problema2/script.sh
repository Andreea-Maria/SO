$1=inputFile.txt

if($1='^https://www\.[a-z]*\.(ro|com);([0-9]{2}|[A-Z]{2}):([0-9]{2}|[A-Z]{2}):([0-9]{2}|[A-Z]{2}):([0-9]{2}|[A-Z]{2}):([0-9]{2}|[A-Z]{2}):([0-9]{2}|[A-Z]{2})$')
	then echo "OK"
	else echo "ERROR"
fi
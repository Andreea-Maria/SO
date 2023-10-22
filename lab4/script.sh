$1=input.txt
$2=output.txt

if  [[ $1 =~ ^[a-zA-Z][a-zA-Z0-9-]*@[a-zA-Z][a-zA-Z0-9]*\.(com|ro|eu)$ && $(echo $1 | tr -d -d '@' | wc -c) -eq 1 ]];
	then echo "OK"
	else
		echo "ERROR"
fi

if [[ $1 =~ ^([1-9][0-9]{0,2}|[1-9][0-9]{0,2}\.[0-9]{1,3}|[1-2][0-4][0-9]{2}|25[0-5][0-5])\.([0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3})$ ]]; 
  then
    echo "OK"
  else
    echo "ERROR"
  fi
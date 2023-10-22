
while read  line
do
	
	grep -E "^[A-Z]{1,1}[A-Za-z0-9]+[_,]*\.$" | grep -v "(si[_]*,)" | grep -v "[n][pb]"
#grep -v "n (p\b)"
done

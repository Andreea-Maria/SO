count=0
count2=0

for entry in $1/*
do
	if [ -f $entry ]
		then res='cat $entry | grep -E "^[0-3][0-9]-[0-1][0-9]|1[0-2])-[1-2][0-9{3} ([0-2][0-9]):([0-5][0-9]):(0-5][0-9])$"' 
			if [ ! -z $res ]
				then 
					echo $entry >> $2
			fi
	elif [ -L $entry]
		then 
			count='expr $count +1'
	elif [ -d $entry ]
		then
			count2='bash $0 $entry $2'
			count='expr $count + $count2'
	fi
			
		
				
echo $count
done


	if(grep -E '^[0-3][0-9]-[0-1][0-9]|1[0-2])-[1-2][0-9{3} ([0-2][0-9]):([0-5][0-9]):(0-5][0-9])$')
		then echo "Data si ora sunt valide"
		else echo "ERROR"
# /bin/bash

for X in $(ls)
do
	if [ -d $X ]
	then
		cd $X
		make clean
		cd ..	
	fi
done
for Y in $(find . -name "process?")
do
	rm $Y
done

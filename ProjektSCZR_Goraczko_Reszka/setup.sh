# /bin/bash

for X in $(ls)
do
	if [ -d $X ]
	then
		cd $X
		make
		ZMIENNA=$(find . -name "process?") 
		ZMIENNA2=${ZMIENNA##*/}
		cp $ZMIENNA2 ..
		cd ..	
	fi
done

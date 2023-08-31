#! /bin/bash

for f_name in "$@"
do
	touch "$f_name"
	if [ $? -eq 0 ]; then
		echo " File named $f_name created "
	else
		echo " Oops..Failed......File Not Created "
	fi
done

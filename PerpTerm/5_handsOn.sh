#!/bin/bash
echo -n "Enter First Number:"
read n1
echo -n "Enter Second Number:"
read n2
if test $n1 -eq $n2
then 
	echo "Same Numbers"
else
	echo "Not Same"
fi
	

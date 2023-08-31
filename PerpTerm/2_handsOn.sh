#! /bin/bash
echo -n "Enter Number:"
read n
for I in {1..100}
do
	echo "$n X $I = `expr $I \* $n`"
done

#! /bin/bash

echo " Enter  1 for listing all shell script files"
echo " Enter  2 for listing all Directories"
echo " Enter your choice: "

read choice
case $choice in
	1)
		ls -l *.sh
		;;
	2)
		ls -l | grep ^d
		;;
	*)
		echo "You have entered a wrong choice"
		;;
esac



#!/bin/bash

FILE="employee.txt"

add_employee() {
  echo "Enter Employee Name:"
  read name
  echo "Enter Employee Number:"
  read emp_num
  echo "Enter Mobile Number:"
  read mob_num

  echo "$name:$emp_num:$mob_num" >> "$FILE"
  echo "Employee record created successfully."
}

delete_employee() {
  echo "Enter the Employee Number to delete:"
  read emp_num
  grep -v "^.*:$emp_num:.*" "$FILE" > temp.txt
  mv temp.txt "$FILE"
  echo "Employee record deleted successfully."
}

search_employee(){
  echo "Enter the Employee Number to search:"
  read emp_num
  grep "^.*:$emp_num:.*" "$FILE"
}

sort_records() {
  sort -t ':' -k 1 "$FILE"
}

list_records() {
  cat "$FILE"
}

while true; 
do
    echo "Hey What you want me to do?"
    echo "Menu:"
    echo "1. Add Employee Record."
    echo "2. Delete Employee Record."
    echo "3. Search Employee Record."
    echo "4. Sort Records."
    echo "5. List All Records."
    echo "6. Exit."

    read -p "Enter your choice: " choice

    case $choice in
        1) add_employee ;;
        2) delete_employee ;;
        3) search_employee ;;
        4) sort_records ;;
        5) list_records ;;
        6) echo "Goodbye!"; exit 0 ;;
        *) echo "Invalid choice. Please try again." ;;
    esac
done

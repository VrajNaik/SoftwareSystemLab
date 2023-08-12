#!/bin/bash

filename="employee.txt"

validate_mobile() {
    if [[ ${#1} -lt 10 ]]; then
        echo "Error: Mobile number should be at least 10 digits."
        return 1
    fi
}

validate_name() {
    if [[ $1 =~ ^[a-zA-Z]+$ ]]; then
        return 0
    else
        echo "Error: Employee name should only consist of letters."
        return 1
    fi
}

add_employee() {
    echo "Enter Employee Name:"
    read name
    validate_name "$name" || return

    echo "Enter Employee Number:"
    read number

    echo "Enter Telephone Number:"
    read tel_number
    validate_mobile "$tel_number" || return

    exists=$(grep "^.*:$number:.*" "$filename")
    if [[ -z $exists ]]; then
        echo "$name:$number:$tel_number" >> "$filename"
        echo "Employee added successfully."
    else
        echo "Error: Employee with this number already exists."
    fi
}

delete_employee() {
  echo "Enter the Employee Number to delete:"
  read emp_num
  grep -v "^.*:$emp_num:.*" "$filename" > temp.txt
  mv temp.txt "$filename"
  echo "Employee record deleted successfully."
}

search_employee() {
    echo "Enter Employee Number to search:"
    read number

    result=$(grep "^.*:$number:.*" "$filename")
    if [[ -z $result ]]; then
        echo "Employee not found."
    else
        echo "Employee Details:"
        echo "$result" | awk -F: '{ printf "Employee Name: %s\n Employee Number: %s\n Employee Mobile Number: %s\n", $1, $2, $3 }'
    fi
}

sort_records() {
    sort -t: -k2 -o "$filename" "$filename"
    echo "Records sorted by Employee Number."
}

list_records() {
    while IFS=: read -r name number tel_number; 
    do
        echo "Employee Name: $name"
        echo "Employee Number: $number"
        echo "Employee Mobile Number: $tel_number"
        echo
    done < "$filename"
}

while true; do
    echo "Menu:"
    echo "1. Add Employee"
    echo "2. Delete Employee"
    echo "3. Search Employee"
    echo "4. Sort Records"
    echo "5. List Records"
    echo "6. Exit"
    echo "Please Enter Your Choice"
    read choice

    case $choice in
        1) add_employee ;;
        2) delete_employee ;;
        3) search_employee ;;
        4) sort_records ;;
        5) list_records ;;
        6) exit ;;
        *) echo "Invalid choice. Please select a valid option." ;;
    esac
done

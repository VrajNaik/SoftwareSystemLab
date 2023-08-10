#!/bin/bash
declare -a employee_records
function add_employee() {
    read -p "Enter Employee Name: " name
    read -p "Enter Employee Number: " emp_number
    read -p "Enter Mobile Number: " phone_number
    
    employee_records+=("$name,$emp_number,$phone_number")
    echo "Employee Record added successfully!"
}
function delete_employee() {
    echo "Enter Employee Number to delete: " 
    read emp_number


    for ((i = 0; i < ${#employee_records[@]}; i++)); 
    do
        record="${employee_records[$i]}"
        if [[ "$record" -eq "$emp_number" ]]; 
        then
            unset 'employee_records[i]'
            echo "Employee Record deleted successfully!"
            return
        fi
    done

    echo "Employee not found! Please Enter Valid Employee Number."
}
function search_employee() {
    echo "Enter Employee Number to search: " 
    read emp_number

    for record in "${employee_records[@]}"; 
    do
        if [[ "$record" -eq "$emp_number" ]]; 
        then
            IFS=',' 
            read -ra details <<< "$record"
            echo "Employee Name: ${details[0]}"
            echo "Employee Number: ${details[1]}"
            echo "Telephone Number: ${details[2]}"
            return
        fi
    done

    "Employee not found! Please Enter Valid Employee Number."
}

function list_records() {
    for record in "${employee_records[@]}"; 
    do
        IFS=',' 
        read -a details <<< "$record"
        echo "Employee Name: ${details[0]}"
        echo "Employee Number: ${details[1]}"
        echo "Telephone Number: ${details[2]}"
        echo
    done
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

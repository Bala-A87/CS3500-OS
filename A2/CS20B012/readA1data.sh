#! /bin/bash

file="Assignment1_data.txt"

if [ -e $file ]; then
    cat $file | 
    tr -sc '[:alpha:]' '[\n*]' |
    sort |
    uniq -c |
    sort -nr |
    head -n 3 
else
    echo "File not found"
fi

#! /bin/bash

upper=0
lower=0
digit=0
symbol=0 #To check whether we find the different types of characters

read -p "Enter the password to check strength of: " password
n=${#password} #Length of the password

if [[ $password =~ .*[A-Z].* ]]; then #Regex check for uppercase letters in the password
    upper=1
fi
if [[ $password =~ .*[a-z].* ]]; then
    lower=1
fi
if [[ $password =~ .*[0-9].* ]]; then
    digit=1
fi
if [[ $password =~ .*[\!\@\#\$\%\^\&\*\(\)\+\-].* ]]; then #Escape sequences since they have special meaning
    symbol=1
fi

all4=$upper+$lower+$digit+$symbol 
mod3=$upper+$lower+$symbol

if [[ $n -ge 8 && $all4 -eq 4 ]]; then
    echo "Password is strong"
elif [[ $n -ge 6 && $mod3 -eq 3 ]]; then
    echo "Password is moderate"
else
    echo "Password is weak"
fi

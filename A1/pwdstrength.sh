#! /bin/sh

upper=0
lower=0
digit=0
symbol=0

read password
n=${#password}

if [[ $password =~ .*[A-Z].* ]]; then
    upper=1
fi
if [[ $password =~ .*[a-z].* ]]; then
    lower=1
fi
if [[ $password =~ .*[0-9].* ]]; then
    digit=1
fi
if [[ $password =~ .*[!@#$%^\&*()-+].* ]]; then #Since & is a symbol in use by the interpreter, needs to be made an escape sequence
    symbol=1
fi

all4=$upper+$lower+$digit+$symbol
mod3=$upper+$lower+$symbol

if [[ $n -ge 8 && $all4 -eq 4 ]]; then
    echo "Strong"
elif [[ $n -ge 6 && $mod3 -eq 3 ]]; then
    echo "Moderate"
else
    echo "Weak"
fi

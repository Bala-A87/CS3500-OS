#!/bin/bash

declare -a arr temp

merge()
{
    if [[ $1 < $2 ]]; then
        local mid=$((($1+$2)/2))
        merge $1 $mid
        merge $(($mid+1)) $2
        
        local i=$1;
        local j=$(($mid+ 1))
        local k=$((0))

        while ((  $i <= $mid  &  $j <= $2  ))
        do
            if [[ $((arr[$i])) < $((arr[$j])) ]]; then
                temp[$k]=$((arr[$i]));
                k=$(($k+1));
                i=$(($i+1));
            else
                temp[$k]=$((arr[$j]));
                k=$(($k+1));
                j=$(($j+1));
            fi
        done

        while (( $i <= $mid ))
        do
            temp[$k]=$((arr[$i]));
            k=$(($k+1));
            i=$(($i+1));
        done
        while (( $j <= $2 ))
        do
            temp[$k]=$((arr[$j]));
            k=$(($k+1));
            j=$(($j+1));
        done

        i=$1;
        k=$((0)); 
        while (( $i <= $2 ))
        do
            arr[$i]=$((temp[$k]));
            i=$(($i+1))
            k=$(($k+1))
        done
    fi
}

#Main code 
read n

for(( c = 0 ; c < $n ; c++ ))
do
    read arr[$c]
    temp[$c]=$arr[$c]
done

merge 0 $(($n-1))

echo -e "${arr[@]}"
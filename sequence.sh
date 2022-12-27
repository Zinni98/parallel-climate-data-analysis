#!/bin/bash

#declare empty array of floats to store the sequence
declare -a sequence

for ((i=3; i<23; i++))
do
    sequence[i]=$(echo "2^$i" | bc -l)
done

#if the bash files named as the sequence elements already exist delete them
for ((i=3; i<23; i++))
do
    if [ -f "${sequence[i]}" ]
    then
        rm "${sequence[i]}"
    fi
done

#use the sequence array to fill with sed other bash files containing a placeholder 
#(in this case the placeholder is the string "PLACEHOLDER")
#the sed command will replace the placeholder and create a new bash file for each element of the sequence
#the new bash files will be named as the sequence elements
for i in "${sequence[@]}"
do
    sed "s/PLACEHOLDER/$i/g" template.sh > "$i.sh"
done

#run in sequence all created bash files waiting for the previous one to finish
#the wait command is used to wait for the previous job to finish
#the & is used to run the job in background
#the jobs command is used to print the list of jobs running in background
for i in "${sequence[@]}"
do
    ./"$i.sh" &
    wait
    jobs
done




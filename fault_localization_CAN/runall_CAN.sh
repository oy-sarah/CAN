#!/bin/sh

for i in $(seq 1 10)  
do   
. ./installDL.sh 
done

gcc -o averageResult analysePro/averageResult.c
./averageResult DLFinalresult.txt
rm -f averageResult

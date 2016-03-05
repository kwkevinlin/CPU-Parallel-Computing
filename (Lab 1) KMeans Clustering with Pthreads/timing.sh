#!/bin/bash
# Compile using the make file
# The bash script will only work if ./main outputs only the result of etime(), no other printf statements

sum=0.0

for i in 1 2 4 8 16 # Threads
do
	for j in 2 5 10 25 # Clusters
	do 
	
		sum=0.0
		echo "P: $i, K: $j"
		var=$(./main $j $i kmeansMedium.txt outputMedium.txt)
		echo $var
		sum=$(echo "scale=5; $sum + $var" | bc)
		var=$(./main $j $i kmeansMedium.txt outputMedium.txt)
		echo $var
		sum=$(echo "scale=5; $sum + $var" | bc)
		var=$(./main $j $i kmeansMedium.txt outputMedium.txt)
		echo $var
		sum=$(echo "scale=5; $sum + $var" | bc)
		echo "scale=5; $sum / 3" | bc
		
	done
done
	
	
#!/bin/bash

#arguments: 1: output filename
#	     2: stable true or false

outputfile="$(realpath "$1")"

SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

echo $outputfile

$SCRIPT_DIR/with_parallel

python3 $SCRIPT_DIR/python/transpo_median.py $outputfile

if [[ $2 == true ]]
then
	if [[ "$outputfile" == *.txt ]]; then
	    stablefile="${outputfile%.txt}_stable.txt"
	elif [[ "$outputfile" == *.dat ]]; then
	    stablefile="${outputfile%.dat}_stable.dat"
	else
	    stablefile="${outputfile}_stable"
	fi
	echo $stablefile
	python3 $SCRIPT_DIR/python/transpo_median.py $stablefile
fi

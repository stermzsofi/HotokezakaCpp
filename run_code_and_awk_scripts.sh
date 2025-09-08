#!/bin/bash

#arguments: 1: output filename
#	     2: stable true or false

outputfile="$(realpath "$1")"

SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

echo $outputfile

$SCRIPT_DIR/with_parallel

transpoout="${outputfile}_transpo"
echo $transpoout
awk -f $SCRIPT_DIR/awk_scripts/transpo.awk $outputfile > $transpoout
medianout="${outputfile}_median"
awk -f $SCRIPT_DIR/awk_scripts/median_calculation_awk.awk $transpoout > $medianout

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
	transpostable="${stablefile}_transpo"
	medianstable="${stablefile}_median"
	awk -f $SCRIPT_DIR/awk_scripts/transpo.awk $stablefile > $transpostable
	awk -f $SCRIPT_DIR/awk_scripts/median_calculation_awk.awk $transpostable > $medianstable
fi


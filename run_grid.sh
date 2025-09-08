#!/bin/bash

R0list=(1.0 5.0 10.0 15.0 20.0 25.0 50.0 100.0 150.0)
taulist=(1.0 5.0 10.0 15.0 30.0 50.0 75.0 100.0)

for R0 in ${R0list[*]}
do
	#echo $R0
	for tau in ${taulist[*]}
	do
		#echo $tau
		outputname=`awk 'BEGIN{printf("r0_%i_tau_%i.dat", '$R0', '$tau')}'`
		#echo $outputname
		awk -v OFS="\t" '{if ($1 == "r0_rate"){print $1, '$R0'} else if($1 == "tau"){print $1, '$tau'}else if($1 == "output"){print $1, "'$outputname'"}else{print $0}}' ../parameters.txt > parameters.txt
		#mv ../temp ../parameters.txt
		if [ ! -f "output_results/"$outputname ]
		then
			echo "Started calculate R0 = " $R0 " tau = " $tau
			../run_code_and_awk_scripts.sh $outputname "true"
			echo "Calculation done R0 = " $R0 " tau = " $tau
			mv $outputname "output_results/"$outputname
			mv *transpo transported/
			mv *median medianfiles/
			mv *stable* output_results/
			
			#transpoout="transported_outputs/"$outputname
			#awk -f ../transpo.awk output_results/$outputname > $transpoout
			#medianout="transported_outputs/median/"$outputname
			#awk -f transported_outputs/median_calculation_awk.awk $transpoout > $medianout
		fi
	done


done



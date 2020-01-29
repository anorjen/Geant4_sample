#!/bin/bash

mkdir -p pic/DET
rm pic/DET/*

for folder in "run4"
do
	#mkdir -p pic/$folder
	#FILE="pic/$folder/hist_info.dat"
	#if [ -f $FILE ]; then
	#	rm $FILE
	#fi
	#touch $FILE
	for var in 0 1
	do
		if [[ $var -eq 0 ]]
		then
			EN=10.0
		elif [[ $var -eq 1 ]]
		then
			EN=100.0
		elif [[ $var -eq 2 ]]
		then
			EN=6000.0
		elif [[ $var -eq 3 ]]
		then
			EN=12000.0
		fi
		#root -l -b -q det_energy.C'("dat/'$folder'/EnergyDeposit_'$var'.dat", '$EN')'
		#root -l -b -q det_start_point.C'("dat/'$folder'/EnergyDeposit_'$var'.dat")'
		root -l -b -q det_lays_energy.C'("dat/'$folder'/EnergyDeposit_'$var'.dat")'
		#root -l -b -q det_cell_counter.C'("dat/'$folder'/EnergyDeposit_'$var'.dat")'
	done
	cp pic/DET/*  pic/$folder/
done

#!/bin/bash

PARTICLE=neutron
AMOUNT=10000
ENERGIES=(10 100)

if [[ $2 != "" ]]
then
	DIR="$2";
else
	DIR="DET";
fi

mkdir -p dat/$DIR
mkdir -p pic/$DIR
mkdir -p pic/DET

#rm -rf dat/$DIR/*;
rm -rf pic/$DIR/*;

if [[ $1 == "1" ]]
then
	cd ../build

	rm -rf Runs/*
	echo "0" > FolderN

	for var in ${ENERGIES[@]}
	do
		echo "/control/execute init.mac
		#/control/execute init_vis.mac

		/gun/particle $PARTICLE

		/gun/energy $var MeV
		/run/beamOn $AMOUNT" > Run.mac
		./det Run.mac
	done

	cd ../root_mac

	for (( var = 0; var < ${#ENERGIES[@]}; var++ ))
	do
		 cp ../build/Runs/$var/EnergyDeposit.dat  dat/$DIR/EnergyDeposit_$var.dat
		 cp ../build/Runs/$var/Hit.log  dat/$DIR/Hit_$var.log
	done
fi

for (( var = 0; var < ${#ENERGIES[@]}; var++ ))
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
	# EN=1000.0
	root -b -q -l det_energy.C'("dat/'$DIR'/EnergyDeposit_'$var'.dat", '$EN')'
	root -b -q -l det_start_point.C'("dat/'$DIR'/EnergyDeposit_'$var'.dat")'
	root -b -q -l det_lays_energy.C'("dat/'$DIR'/Hit_'$var'.log")'
done

if [[ $DIR != "DET" ]]
then
	mv pic/DET/* pic/$DIR/;
fi


#!/bin/sh
if [ $# -ne 1 ]; then
	echo "usage: pf-gantt [input-file.csv]"
	exit
fi
./pf < $1
python Gantt/gantt.py -o gantt.gpl -c Gantt/colors/colors.cfg -a -t $1 gantt.txt
#python Gantt/gantt.py -o gantt.gpl --palette=Gantt/colors/se_darkpalette.dat -t $1 gantt.txt
gnuplot -persist gantt.gpl

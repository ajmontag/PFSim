#!/bin/sh
if [ $# -ne 1 ]; then
	echo "usage: pf-gantt [input-file.csv]"
	exit
fi
./pf < $1
python Gantt/gantt.py -o gantt.gpl gantt.txt
gnuplot -persist gantt.gpl

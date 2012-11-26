#!/bin/sh
./pf < $1
python Gantt/gantt.py -o gantt.gpl gantt.txt
gnuplot -persist gantt.gpl

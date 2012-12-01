PFSim
=====

A simulator of the PF P-Fair real-time periodic-task scheduling algorithm for multiprocessors.


*How To:*
Compile:
  cd src
  make

Run:  
  ./pf < ../input/sample-a.csv
  
Generate and show a Gantt Chart:
  # note: you will need python and gnuplot
  ./pf-gantt ../input/sample-a.csv



*Repository Contents*

src
source code.

input
sample input files.

results
scheduling results of the sample input files. 

results-img
images of gnuplot gantt charts from the sample input files. 


See also: 
[Final Report](https://docs.google.com/document/d/1VwEJmEhah9cgEDX4tTj7JmRTSFGWlVNVInedhLjHhPM/edit) describing the project and its results. 


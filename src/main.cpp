/**
 * PFair
 * 
 * File: 
 *   main.cpp
 * Authors:
 *   Andrew Montag, Austin Laugen
 * License:
 *   Boost 1.0 <http://www.boost.org/users/license.html>
 */

#include "InputParameters.hpp"
#include "Schedule.hpp"
#include "AlgoPF.hpp"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std; 
using namespace pfair; 

bool pfverbose = false; 

void outputMaxLags(std::ostream& os, const std::vector<Task>& tasks, const MaxLagVect& maxLags)
{
    os << "Max Lags: \n";
    for (int i = 0; i < tasks.size(); ++i) {
        os << tasks[i].alias_ << ": " << maxLags[i].lag << " @ " << maxLags[i].t << std::endl; 
    }
}

void outputSchedule(std::ostream& os, const std::vector<Task>& tasks, const Schedule& s)
{
    os << "time\t"; 
    for (std::vector<Task>::const_iterator taski = tasks.begin(); taski != tasks.end(); ++taski) {
        os << (*taski).alias_ << '\t';
    }
    os << std::endl << s << std::endl; 
}

int main (int argc, char** argv) 
{
    InputParameters param; 

    try {
        cin >> param; 
    } catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl; 
    }

    // print stuff for testing 
    if (pfverbose) cout << param << endl; 
        
    MaxLagVect maxLags(param.numTasks()); 
    Schedule s(param.numResources(), param.scheduleTime(), param.numTasks()); 

    const clock_t tbegin = clock(); 
    {
        algoPF(s, param.tasks(), maxLags); 
    }
    const clock_t tend = clock();
    const double elapsed_secs = double(tend - tbegin) / (double) CLOCKS_PER_SEC;

    cout << "Completed Algo PF in " << elapsed_secs << " seconds.\nSchedule:\n";
    
    outputSchedule(std::cout, param.tasks(), s);
    outputMaxLags(std::cout, param.tasks(), maxLags);  

    ofstream ganttFile("gantt.txt");
    s.createPlotData(ganttFile, param.tasks());

    return 0; 
}

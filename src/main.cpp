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

void outputMaxLags(std::ostream& os, const std::vector<Task>& tasks, const std::vector<float>& maxLags)
{
    os << "Max Lags: \n";
    for (int i = 0; i < tasks.size(); ++i) {
        os << tasks[i].alias_ << ": " << maxLags[i] << std::endl; 
    }
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
    cout << param << endl; 

    Schedule s(param.numResources(), param.scheduleTime(), param.numTasks()); 
    std::vector<float> maxLags(param.numTasks(), 0.0f); 

    algoPF(s, param.tasks(), maxLags); 

    // TODO also find max lag for each task

    cout << "Completed Algo PF. Schedule: \n";
    cout << s << endl;

    outputMaxLags(std::cout, param.tasks(), maxLags);  

    ofstream ganttFile("gantt.txt");
    s.createPlotData(ganttFile, param.tasks());

    return 0; 
}

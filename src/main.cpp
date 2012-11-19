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

using namespace std; 
using namespace pfair; 

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

    algoPF(s, param.tasks()); 

    std::cout << "Completed Algo PF. Schedule: \n";
    std::cout << s << std::endl; 

    return 0; 
}

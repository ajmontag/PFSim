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

    return 0; 
}

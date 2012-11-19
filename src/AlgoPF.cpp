#include "AlgoPF.hpp"

namespace pfair { 

void algoPF(Schedule& s, const std::vector<Task>& tasks)
{
    // for each time unit
    for (int t = 0; t < s.duration(); ++t, s.tick()) {
        // for each task
        for (int i = 0; i < tasks.size(); ++i) {
            const Task& task = tasks[i]; 

            // calculate lag
            // TODO 
            
            // calculate a
            // TODO 

            // calculate ahead/behind
            // TODO 

            // calculate urgent/tnegru/contending
            // TODO 

        } // end for each task

        // schedule all urgent tasks
        // TODO 

        // if contending tasks exist
        // TODO 
        if (1) {
            // determine total ordering 
            // TODO 

            // schedule on the available resources in order
            // TODO 
        }

    } // end for each time unit

}

} // end namespace pfair

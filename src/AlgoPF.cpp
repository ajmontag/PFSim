#include "AlgoPF.hpp"

namespace pfair { 

typedef enum { NONE, AHEAD, BEHIND } ePace; 
typedef enum { NONE, URGENT, TNEGRU, CONTENDING } eUrgency; 

void algoPF(Schedule& s, const std::vector<Task>& tasks)
{
    // for each time unit
    for (int t = 0; t < s.duration(); ++t, s.tick()) {
        // XXX i think these will reset every iteration? it may not matter...
        std::vector<float> lag(tasks.size(), 0.0f);
        std::vector<char> alpha(tasks.size(), '0');
        std::vector<ePace> pace(tasks.size(), NONE);
        std::vector<eUrgency> urgency(tasks.size(), NONE);

        // for each task
        for (int i = 0; i < tasks.size(); ++i) {
            const Task& task = tasks[i]; 

            // calculate lag
            lag[i] = task.weight * t - s.prevScheduled(); 

            // calculate alpha
            // TODO 

            // calculate ahead/behind
            // shortcut: lemma 4.5?
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

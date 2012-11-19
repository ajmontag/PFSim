#include "AlgoPF.hpp"

namespace pfair { 

void algoPF(Schedule& s, const std::vector<Task>& tasks)
{
    // for each time unit
    for (int t = 0; t < s.duration(); ++t, s.tick()) {
        // for each task
        for (int i = 0; i < tasks.size(); ++i) {
            const Task& task = tasks[i]; 

            // TODO 

        } // end for each task

    } // end for each time unit

}

} // end namespace pfair

#include "AlgoPF.hpp"
#include <cmath>
#include <sstream>

namespace pfair { 

typedef enum { NONE_PACE, AHEAD, BEHIND, PUNCTUAL } ePace; 
typedef enum { NONE_URGENCY, URGENT, TNEGRU, CONTENDING } eUrgency; 

inline char signChar(float f) 
{
    if (f > 0.0f) return '+'; 
    if (f < 0.0f) return '-'; 
    else return '0'; 
}

void algoPF(Schedule& s, const std::vector<Task>& tasks)
{
    // for each time unit
    for (int t = 0; t < s.duration(); ++t, s.tick()) {
        // XXX i think these will reset every iteration? it may not matter...
        std::vector<float> lag(tasks.size(), 0.0f);
        std::vector<char> alpha(tasks.size(), '0');
        std::vector<ePace> pace(tasks.size(), NONE_PACE);
        std::vector<eUrgency> urgency(tasks.size(), NONE_URGENCY);
        // OPTIMIZE we may be able to remove these vectors and just use the result in the calc below

        bool contendingTaskExists = false; 

        // for each task
        for (int i = 0; i < tasks.size(); ++i) {
            const Task& task = tasks[i]; 

            // calculate lag
            lag[i] = task.weight_ * t - s.prevScheduled(i); 

            // calculate alpha, see bottom of page 607
            // a[t](x)  =  sign(x.w * (t  +  1) - floor( x.w * t) - 1)
            alpha[i] = signChar(task.weight_ * (t + 1) - floor(task.weight_ * t) - 1.0f);

            // calculate ahead/behind/punctual
            // TODO shortcut: lemma 4.5? 4.2?
            //  task x is ahead if and only if lag(S, x, t) < 0, 
            //  that task x is behind if and only if lag(S, x, t) > 0, 
            //  and that task x is punctual if and only if it is neither ahead nor behind.
            if (lag[i] < 0.0f) 
                pace[i] = AHEAD; 
            else if (lag[i] > 0.0f)
                pace[i] = BEHIND; 
            else 
                pace[i] = PUNCTUAL; 
            
            // calculate urgent/tnegru/contending
            //  task x is tnegru if and only if x is ahead and alpha[t](x) !=  '+', 
            //  that task x is urgent if and only if x is behind and alpha[t](x) != '-', 
            //  and that task x is contending if and only if it is neither tnegru nor urgent.
            if (AHEAD == pace[i] && '+' != alpha[i]) {
                urgency[i] = TNEGRU; 
            } else if (BEHIND == pace[i] && '-' != alpha[i]) {
                urgency[i] = URGENT; 
            } else { 
                urgency[i] = CONTENDING; 
                contendingTaskExists = true; 
            }
        
        } // end for each task

        int resourcesAvailable = s.resourceCount();

        // schedule all urgent tasks
        // OPTIMIZE could put this in the loop above
        for (int i = 0; i < tasks.size(); ++i) {
            if (URGENT == urgency[i]) {
                s.set(i); 
                resourcesAvailable -= 1;
            }
        }

        if (resourcesAvailable < 0) {
            std::stringstream ss; 
            ss << "There are more urgent tasks than available resources!\ntime = " << t << " Difference = " << resourcesAvailable; 
            throw SchedulingException(ss.str());
        }

        // if contending tasks exist and resources are available
        if (contendingTaskExists && resourcesAvailable > 0) {
            // determine total ordering 
            // TODO 
            //  x > y if and only if alpha(x, t) >= alpha(y, t), 
            //  where the comparison between characteristic substrings alpha(x, t) and alpha(y, t) 
            //  is resolved lexicographically with - < 0 <  +.

            // schedule on the available resources in order
            // TODO 
        }

    } // end for each time unit

}

} // end namespace pfair

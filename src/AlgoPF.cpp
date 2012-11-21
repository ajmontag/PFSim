#include "AlgoPF.hpp"
#include <cmath>
#include <sstream>
#include <queue>

namespace pfair { 

typedef enum { NONE_PACE, AHEAD, BEHIND, PUNCTUAL } ePace; 
typedef enum { NONE_URGENCY, URGENT, TNEGRU, CONTENDING } eUrgency; 
typedef enum { NONE_CHAR = 0, MINUS = 1, ZERO = 2, PLUS = 3 } eChar; // makes compare easier

inline eChar signChar(float f) 
{
    if (f > 0.0f) return PLUS; 
    if (f < 0.0f) return MINUS; 
    else return ZERO; 
}

/**
 * push more chars onto the characteristic substring, until a 0 is reached
 */
inline void computeCharSubstring(std::queue<eChar>& q, float w, int t)
{
    eChar c; 
    do {
        c = signChar(w * (t + 1) - floor(w * t) - 1.0f);
        q.push(c); 
        t += 1; 
    } while (ZERO != c);
}

template <typename T>
inline void popAll(std::vector<std::queue<T> >& v) 
{
    for (int i = 0; i < v.size(); ++i) {
        v[i].pop(); 
    }
}

/**
 * Sorts contendingTasks in lexiographic order based on their characteristicSubstring
 */
inline void lexSort(std::vector<int>& contendingTasks, const std::vector<std::queue<eChar> >& cs)
{
    // TODO 
}

void algoPF(Schedule& s, const std::vector<Task>& tasks)
{
    // we could also compute the entire characteristic string for the entire schedule duration
    std::vector<std::queue<eChar> > cs(tasks.size()); // characteristicSubstrings

    // for each time unit
    for (int t = 0; t < s.duration(); ++t, s.tick(), popAll(cs)) {
        // XXX i think these will reset every iteration? it may not matter...
        std::vector<float> lag(tasks.size(), 0.0f);
        //std::vector<char> alpha(tasks.size(), '0');
        std::vector<ePace> pace(tasks.size(), NONE_PACE);
        std::vector<eUrgency> urgency(tasks.size(), NONE_URGENCY);
        // OPTIMIZE we may be able to remove these vectors and just use the result in the calc below
        std::vector<int> contendingTasks; // the task ids of all contending tasks
        contendingTasks.clear();

        // for each task
        for (int i = 0; i < tasks.size(); ++i) {
            const Task& task = tasks[i]; 

            // calculate lag
            lag[i] = task.weight_ * t - s.prevScheduled(i); 

            // calculate alpha, see bottom of page 607
            // a[t](x)  =  sign(x.w * (t  +  1) - floor( x.w * t) - 1)
            //alpha[i] = signChar(task.weight_ * (t + 1) - floor(task.weight_ * t) - 1.0f);
            
            // if the substring is empty, compute more up to the next '0'
            if (cs[i].empty()) {
                computeCharSubstring(cs[i], task.weight_, t);
            }
            const eChar alpha = cs[i].front(); 

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
            if (AHEAD == pace[i] && PLUS != alpha) {
                urgency[i] = TNEGRU; 
            } else if (BEHIND == pace[i] && MINUS != alpha) {
                urgency[i] = URGENT; 
            } else { 
                urgency[i] = CONTENDING; 
                contendingTasks.push_back(i); 
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
        if (!contendingTasks.empty() && resourcesAvailable > 0) {
            // determine total ordering 
            // TODO 
            //  x > y if and only if alpha(x, t) >= alpha(y, t), 
            //  where the comparison between characteristic substrings alpha(x, t) and alpha(y, t) 
            //  is resolved lexicographically with - < 0 <  +.
            lexSort(contendingTasks, cs);

            // schedule on the available resources in order
            for (std::vector<int>::iterator iter = contendingTasks.begin(); 
                iter != contendingTasks.end() && resourcesAvailable > 0; 
                ++iter, resourcesAvailable -= 1) 
            {
                s.set(*iter);    
            }
        }

    } // end for each time unit

}

} // end namespace pfair

#include "AlgoPF.hpp"
#include <cmath>
#include <sstream>
#include <deque>
#include <algorithm>
#include <vector>

extern bool pfverbose; 

namespace pfair { 

typedef enum { NONE_PACE, AHEAD, BEHIND, PUNCTUAL } ePace; 
typedef enum { NONE_URGENCY, URGENT, TNEGRU, CONTENDING } eUrgency; 
typedef enum { NONE_CHAR = 0, MINUS = 1, ZERO = 2, PLUS = 3 } eChar; // makes compare easier

//debugging function
inline void printSubstrings(std::vector<int>& contendingTasks, const std::vector<std::deque<eChar> >& cs)
{
    int taskNumber;
    for(int i = 0; i < contendingTasks.size(); i++)
    {
        taskNumber = contendingTasks[i];
        std::cerr << taskNumber << " -> ";
        std::cerr << cs[taskNumber].size() << "--------";

        for(int j = 1; j < cs[taskNumber].size(); j++)
        {
            std::cerr << cs[taskNumber][j];
        }
        std::cerr << std::endl;
    }
    std::cerr <<"------------" << std::endl;
}
    
// comparator for comparing lexicographical strings
struct CompareSubstring : std::binary_function<int, int, bool> 
{
    //Notes/references
        //"with +  having  priority  over  0,  and  0  over -" pg. 616

    private:
        //need access to the substrings to compare
        const std::vector<std::deque<eChar> >& cs;
    public:
        //constructor to get substrings
        CompareSubstring(const std::vector<std::deque<eChar> >& csIn): cs(csIn) {};
    
    bool operator() (int i,int j) const
    {

        //start at 1 to begin comparing substrings at t+1
        int counter = 1;
        
        //loop through and compare substrings until difference is found or the end of either substring is reached
        while(counter < cs[i].size() && counter < cs[j].size())
        {
            //i's character is strictly greater than j's character
            if(cs[i][counter] > cs[j][counter])
            {
                return true;
            }
            //i's character is strictly less than j's character
            else if(cs[i][counter] < cs[j][counter])
            {
                return false;
            }
            
            //they're the same - go to next character
            counter += 1;
        }
        //if hasn't returned yet, strings are the same up to one (or both) running out of characters
        
        //if i is shorter than j, it comes first in lexicographical order
        if(cs[i].size() < cs[j].size())
        {
            return true;
        }
        
        //same length OR j is shorter - return false either way
        return false;
        
    
    }
};
    
inline eChar signChar(float f) 
{
    if (f > 0.0f) return PLUS; 
    if (f < 0.0f) return MINUS; 
    else return ZERO; 
}

/**
 * push more chars onto the characteristic substring, until a 0 is reached OR the end of the schedule is reached
 */
inline void computeCharSubstring(std::deque<eChar>& q, float w, int t, Schedule &s)
{
    eChar c; 
    do {
        c = signChar(w * (t + 1) - floor(w * t) - 1.0f);
        q.push_back(c); 
        t += 1; 
    } while (ZERO != c && t < s.duration());
}

template <typename T>
inline void popAll(std::vector<std::deque<T> >& v) 
{
    for (int i = 0; i < v.size(); ++i) {
        v[i].pop_front(); 
    }
}

/**
 * Sorts contendingTasks in lexiographic order based on their characteristicSubstring
 */
inline void lexSort(std::vector<int>& contendingTasks, const std::vector<std::deque<eChar> >& cs)
{
    //sort the vector
    std::sort(contendingTasks.begin(), contendingTasks.end(), CompareSubstring(cs));

    //Debugging - print out substrings
    if (pfverbose) printSubstrings(contendingTasks, cs);

}

void algoPF(Schedule& s, const std::vector<Task>& tasks, MaxLagVect& maxLags)
{
    // we could also compute the entire characteristic string for the entire schedule duration
    //cs index 0 is current time, 1 is t+1, etc (add at back)
    std::vector<std::deque<eChar> > cs(tasks.size()); // characteristicSubstrings

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

            // store max lag
            if (lag[i] > maxLags[i].lag)
                maxLags[i] = MaxLagPair(t, lag[i]);

            // calculate alpha, see bottom of page 607
            // a[t](x)  =  sign(x.w * (t  +  1) - floor( x.w * t) - 1)
            //alpha[i] = signChar(task.weight_ * (t + 1) - floor(task.weight_ * t) - 1.0f);
            
            // if the substring is empty OR only has 1 element, compute more up to the next '0'
            //NOTE: If it only has 1 element it will need more in case tasks need to be compared and sorted
            if (cs[i].size() <= 1) {
                computeCharSubstring(cs[i], task.weight_, t + cs[i].size(), s);
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

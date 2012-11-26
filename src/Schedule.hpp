#ifndef _PFSIM_SCHEDULE_HPP_
#define _PFSIM_SCHEDULE_HPP_

#include <vector>
#include <iostream>
#include "Task.hpp"

namespace pfair {

class SchedulingException : public std::exception {
public:
    SchedulingException(const std::string& msg) : msg_(msg) { /* do nothing */ } 
    virtual ~SchedulingException() throw() { /* do nothing */ }
    virtual const char* what() const throw() { return msg_.c_str(); }
private:
    const std::string msg_; 
}; 

class Schedule {
public:
    Schedule(int m, int duration, int taskCount);
    virtual ~Schedule() { } 

    void tick(); 
    
    void set(int taskID, bool isScheduled = true);

    // provides the last term of lag: sum i in [0, t) S(x, i) 
    // in constant time
    int prevScheduled(int taskID) const; 

    int currentTime() const; 

    int duration() const; 

    int resourceCount() const; 

    void createPlotData(std::ostream& os, const std::vector<Task>& tasks) const;

    friend std::ostream& operator<<(std::ostream& os, const Schedule& rhs); 

private:

    // isScheduled = s_[time][task]
    std::vector<std::vector<bool> > s_; 
    std::vector<int> prevScheduled_; // provides the last term for lag sum(x, i)
    int currentTime_; 
    int duration_; 
    int m_;
};

inline Schedule::Schedule(int m, int duration, int taskCount) :
    s_(duration, std::vector<bool>(taskCount, false)), prevScheduled_(std::vector<int>(taskCount, 0)), 
    currentTime_(0), duration_(duration), m_(m)
{
    // do nothing
}

inline void Schedule::tick()
{
    currentTime_ += 1;
}

inline void Schedule::set(int taskID, bool isScheduled)
{
    s_[currentTime_][taskID] = isScheduled; 
    if (isScheduled)
        prevScheduled_[taskID] += 1; 
}

inline int Schedule::prevScheduled(int taskID) const
{
    return prevScheduled_[taskID]; 
}

inline int Schedule::currentTime() const
{
    return currentTime_;
}

inline int Schedule::duration() const
{
    return duration_; 
}

inline int Schedule::resourceCount() const
{
    return m_; 
}

inline void Schedule::createPlotData(std::ostream& os, const std::vector<Task>& tasks) const
{
    // http://se.wtb.tue.nl/sewiki/wonham/gantt.py
    // resource start-time end-time task

    // convert from isScheduled = s_[time][task]
    //std::vector<> resource(m_);
    for (int t = 0; t < duration_; ++t) {
        int resource;
        resource = 0;  
        for (int task = 0; task < s_[t].size(); ++task) {
            if (s_[t][task]) {
                os << resource << " " << t << " " << (t + 1);
                os << " " << tasks[task].alias_ << '\n';
                resource = (resource + 1) % m_; 
            }
        }
    }
}

// write schedule to the stream (stdout)
inline std::ostream& operator<<(std::ostream& os, const Schedule& rhs)
{
    os << "TODO make this operator output the schedule in the correct format\n";
    os << "time\ttasks...\n";
    for (int t = 0; t < rhs.s_.size(); ++t) {
        os << t << '\t'; 
        for (int i = 0; i < rhs.s_[0].size(); ++i) {
            os << (rhs.s_[t][i] ? "1\t" : "0\t");
        }
        os << '\n';
    }
    return os; 
}

} // end namespace pfair

#endif //_PFSIM_SCHEDULE_HPP_

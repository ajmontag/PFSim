/**
 * PFair
 * 
 * File: 
 *   InputParameters.hpp
 * Authors:
 *   Andrew Montag, Austin Laugen
 * License:
 *   Boost 1.0 <http://www.boost.org/users/license.html>
 */

#include <cstdio>
#include <iostream>
#include <vector>
#include <string>

namespace pfair {

/*
An input file is of the form:

3               # number of resources
20              # number of time units for which to produce a schedule
v, 1, 3         # task, execution requirement, period
w, 2, 4         # ... 
x, 5, 7
y, 8, 11
z, 335, 462

*/

class MalformedInputException : public std::exception {
public:
    MalformedInputException(const std::string& msg) : msg_(msg) { /* do nothing */ } 
    virtual ~MalformedInputException() throw() { /* do nothing */ }
    virtual const char* what() const throw() { return msg_.c_str(); }
private:
    const std::string msg_; 
}; 


class Task {
public:
    std::string alias_;
    int exec_; 
    int period_; 
    float weight_; 
};


class InputParameters {
public:
    InputParameters() : numResources_(0), scheduleTime_(0) { /* do nothing */ } 
    ~InputParameters() { /* do nohting */ } 

    /* Getters */ 
    const std::vector<Task>& tasks() const { return tasks_; }
    int numResources() const { return numResources_; }
    int scheduleTime() const { return scheduleTime_; }

    /**
     * Fill the structure from the (formatted) stream. 
     * @throws MalformedInputException
     */ 
    friend std::istream& operator>>(std::istream& is, InputParameters& param); 
    
    /**
     * Output general information to a steam
     */
    friend std::ostream& operator<<(std::ostream& os, const InputParameters& param); 

private:

    int numResources_; 
    int scheduleTime_; 
    std::vector<Task> tasks_;  
};

//////
// Implementation
//////

inline std::istream& operator>>(std::istream& is, Task& t)
{
    is >> t.alias_;
    t.alias_.erase(t.alias_.size() - 1); 
    is >> t.exec_;
    is.ignore(1 , ',');
    if (!is) throw MalformedInputException("input file is bad in Task"); 
    is >> t.period_;
    t.weight_ = (float) t.exec_ / (float) t.period_; 
    return is; 
}

inline std::istream& operator>>(std::istream& is, InputParameters& param)
{
    if (!is) throw MalformedInputException("input file is bad"); 
    is >> param.numResources_;
    is >> param.scheduleTime_;
    if (!is) throw MalformedInputException("Malformed Input before task list");
    while (is.peek() != EOF) {
        param.tasks_.push_back(Task()); 
        is >> param.tasks_.back(); 
        is.ignore(1, '\n'); 
    }
    return is; 
}

std::ostream& operator<<(std::ostream& os, const Task& t)
{
    os << "[" << t.alias_ << ", " << t.exec_ << ", " << t.period_ << ", " << t.weight_ << "]"; 
    return os;
}

std::ostream& operator<<(std::ostream& os, const InputParameters& param)
{
    os << "NumResources = " << param.numResources_ << std::endl; 
    os << "ScheduleTime = " << param.scheduleTime_ << std::endl;
    os << "Scheduling for tasks:\n\t"; 
    for (std::vector<Task>::const_iterator i = param.tasks_.begin(); i != param.tasks_.end(); ++i) {
        os << (*i) << " & ";      
    } 
    return os; 
}

} // end namespace pfair

#ifndef _PFSIM_ALGO_PF_HPP_
#define _PFSIM_ALGO_PF_HPP_

#include "Schedule.hpp"
#include "Task.hpp"
#include <vector>

namespace pfair {

class MaxLagPair {
public:
    MaxLagPair(int t_ = 0, float lag_ =0.0f ) : t(t_), lag(lag_) { }
    int t; 
    float lag; 
};

typedef std::vector<MaxLagPair> MaxLagVect;

void algoPF(Schedule& s, const std::vector<Task>& tasks, MaxLagVect& maxLags);

} // end namespace pfair

#endif // _PFSIM_ALGO_PF_HPP_

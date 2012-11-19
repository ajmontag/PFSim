#ifndef _PFSIM_ALGO_PF_HPP_
#define _PFSIM_ALGO_PF_HPP_

#include "Schedule.hpp"
#include "Task.hpp"
#include <vector>

namespace pfair {

void algoPF(Schedule& s, const std::vector<Task>& tasks);

} // end namespace pfair

#endif // _PFSIM_ALGO_PF_HPP_

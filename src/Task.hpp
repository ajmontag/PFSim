#ifndef _PFSIM_TASK_HPP_
#define _PFSIM_TASK_HPP_


namespace pfair {

class Task {
public:
    int id; 
    std::string alias_;
    int exec_; 
    int period_; 
    float weight_; 
};

} // end namespace pfair

#endif //_PFSIM_TASK_HPP_

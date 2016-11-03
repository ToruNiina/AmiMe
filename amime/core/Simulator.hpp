#ifndef AMIME_CORE_SIMULATOR
#define AMIME_CORE_SIMULATOR
#include <amime/core/Network.hpp>

namespace amime
{

template<typename traitsT>
class Simulator
{
  public:
    typedef traitsT traits_type;
    typedef typename traits_type::time_type time_type;
    typedef typename traits_type::state_type state_type;
    typedef typename traits_type::solver_type solver_type;

    typedef Network<traits_type> network_type;
    typedef typename network_type::node_type node_type;
    typedef typename network_type::node_id_type node_id_type;

  public:
    Simulator(): time_(0), end_time_(std::numeric_limits<time_type>::max()){}
    ~Simulator() = default;

    bool step() {this->step_(); return time_ < end_time_;}
    void step_until(const time_type interval);

  private:
    void step_();

  private:
    time_type   time_;
    time_type   end_time_;
    solver_type solver_;
    network_type network_;
};


} // amime
#endif /* AMIME_CORE_SIMULATOR */

#ifndef AMIME_CORE_SIMULATOR
#define AMIME_CORE_SIMULATOR
#include <amime/core/Network.hpp>

namespace amime
{

template<typename traitsT, template<typename T>class solverT>
class Simulator
{
  public:
    typedef traitsT traits_type;
    typedef solverT<traits_type> solver_type;
    typedef typename traits_type::time_type time_type;
    typedef typename traits_type::state_type state_type;

    typedef Network<traits_type> network_type;
    typedef typename network_type::node_type node_type;
    typedef typename network_type::node_id_type node_id_type;

  public:
    Simulator(const time_type end_time,
              const solver_type& solver, network_type& net)
        : time_(0), end_time_(end_time), solver_(solver), network_(net)
    {}
    ~Simulator() = default;

    bool step(){this->step_(); return (time_ < end_time_);}
    void step_until(const time_type t){while(this->time < t){this->step_();}}

    time_type  time() const {return this->time_;}
    time_type& time()       {return this->time_;}
    network_type const& network() const {return network_;}
    network_type &      network()       {return network_;}
    solver_type const& solver() const {return solver_;}
    solver_type &      solver()       {return solver_;}

  private:
    void step_(){this->time_ = this->solver_(this->time_, network_);}

  private:
    time_type     time_;
    time_type     end_time_;
    solver_type   solver_;
    network_type& network_;
};

} // amime
#endif /* AMIME_CORE_SIMULATOR */

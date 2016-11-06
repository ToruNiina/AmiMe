#ifndef AMIME_CORE_RUNGE_KUTTA_ODE_SOLVER
#define AMIME_CORE_RUNGE_KUTTA_ODE_SOLVER
#include <amime/core/Network.hpp>
#include <amime/util/zip_iterator.hpp>
#include <amime/util/make_zip.hpp>
#include <iostream>

namespace amime
{

template<typename traitsT>
class RungeKuttaODESolver
{
  public:
    typedef traitsT traits_type;
    typedef typename traits_type::time_type time_type;
    typedef typename traits_type::state_type state_type;

    typedef Network<traits_type> network_type;
    typedef typename network_type::node_type node_type;
    typedef typename network_type::node_id_type node_id_type;

  public:

    RungeKuttaODESolver(const time_type delta_t): dt_(delta_t){}
    ~RungeKuttaODESolver() = default;

//     void update(const network_type& net);
    time_type operator()(const time_type current, network_type& net);

    time_type  dt() const {return dt_;}
    time_type& dt()       {return dt_;}

  private:
    time_type dt_;
    std::vector<state_type> init;
    std::vector<state_type> k1;
    std::vector<state_type> k2;
    std::vector<state_type> k3;
    std::vector<state_type> k4;
};

template<typename traitsT>
typename RungeKuttaODESolver<traitsT>::time_type
RungeKuttaODESolver<traitsT>::operator()(
        const time_type current, network_type& net)
{
    // store initial values
    this->init.reserve(net.size()); this->init.clear();
    for(auto iter = net.cbegin(); iter != net.cend(); ++iter)
        init.push_back(iter->second.state);

    // calc k1 and partially update the states
    this->k1.reserve(net.size()); this->k1.clear();
    for(auto iter = net.cbegin(); iter != net.cend(); ++iter)
        k1.push_back(iter->second.reaction(current, iter->second.state, iter->second.inputs));

    for(auto iter = make_zip(net.begin(), init.cbegin(), k1.cbegin());
            iter != make_zip(net.end(), init.cend(), k1.cend()); ++iter)
        get<0>(iter)->second.state = *(get<1>(iter)) + *(get<2>(iter)) * this->dt_ * 0.5;

    // calc k2 and partially update the states
    this->k2.reserve(net.size()); this->k2.clear();
    for(auto iter = net.cbegin(); iter != net.cend(); ++iter)
        k2.push_back(iter->second.reaction(
                    current + this->dt_ * 0.5, iter->second.state, iter->second.inputs));

    for(auto iter = make_zip(net.begin(), init.cbegin(), k2.cbegin());
            iter != make_zip(net.end(), init.cend(), k2.cend()); ++iter)
        get<0>(iter)->second.state = *(get<1>(iter)) + *(get<2>(iter)) * this->dt_ * 0.5;

    // calc k3 and partially update the states
    this->k3.reserve(net.size()); this->k3.clear();
    for(auto iter = net.cbegin(); iter != net.cend(); ++iter)
        k3.push_back(iter->second.reaction(
                    current + this->dt_ * 0.5, iter->second.state, iter->second.inputs));

    for(auto iter = make_zip(net.begin(), init.cbegin(), k3.cbegin());
            iter != make_zip(net.end(), init.cend(), k3.cend()); ++iter)
        get<0>(iter)->second.state = *(get<1>(iter)) + *(get<2>(iter)) * this->dt_;

    // calc k4
    this->k4.reserve(net.size()); this->k4.clear();
    for(auto iter = net.cbegin(); iter != net.cend(); ++iter)
        k4.push_back(iter->second.reaction(
                    current + this->dt_, iter->second.state, iter->second.inputs));

    // update all
    for(auto iter = make_zip(net.begin(), init.cbegin(),
                             k1.cbegin(), k2.cbegin(), k3.cbegin(), k4.cbegin());
            iter != make_zip(net.end(), init.cend(),
                             k1.cend(), k2.cend(), k3.cend(), k4.cend()); ++iter)
        get<0>(iter)->second.state = *(get<1>(iter)) + this->dt_ * (1. / 6.) *
                                    (*(get<2>(iter)) + *(get<3>(iter)) * 2 +
                                     *(get<4>(iter)) * 2 + *(get<5>(iter)));
   
    return current + dt_;
}


} // amime
#endif /* AMIME_CORE_RUNGE_KUTTA_ODE_SOLVER */

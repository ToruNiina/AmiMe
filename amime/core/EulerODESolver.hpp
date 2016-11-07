#ifndef AMIME_CORE_EULER_ODE_SOLVER
#define AMIME_CORE_EULER_ODE_SOLVER
#include <amime/core/Network.hpp>
#include <amime/util/zip_iterator.hpp>
#include <amime/util/make_zip.hpp>

namespace amime
{

template<typename traitsT>
class EulerODESolver
{
  public:
    typedef traitsT traits_type;
    typedef typename traits_type::time_type time_type;
    typedef typename traits_type::state_type state_type;

    typedef Network<traits_type> network_type;
    typedef typename network_type::node_type node_type;
    typedef typename network_type::node_id_type node_id_type;

  public:

    EulerODESolver(const time_type delta_t): dt_(delta_t){}
    ~EulerODESolver() = default;

//     void update(const network_type& net);
    time_type operator()(const time_type current, network_type& net);

    time_type  dt() const {return dt_;}
    time_type& dt()       {return dt_;}

  private:
    time_type dt_;
    std::vector<state_type> k;
};

template<typename traitsT>
typename EulerODESolver<traitsT>::time_type
EulerODESolver<traitsT>::operator()(const time_type current, network_type& net)
{
    this->k.reserve(net.size()); this->k.clear();
    for(auto iter = net.cbegin(); iter != net.cend(); ++iter)
        this->k.push_back(iter->second.reaction(
                    current, iter->second.state, iter->second.inputs));

    for(auto iter = make_zip(net.begin(), this->k.cbegin());
            iter != make_zip(net.end(), this->k.cend()); ++iter)
        get<0>(iter)->second.state += *(get<1>(iter)) * this->dt_;

    return current + this->dt_;
}

} // amime
#endif /* AMIME_CORE_EULER_ODE_SOLVER */

#ifndef AMIME_CORE_NODE
#define AMIME_CORE_NODE
#include <vector>
#include <functional>

namespace amime
{

template<typename stateT, typename timeT>
struct Node
{
    typedef std::vector<Node const*> neighbor_type;
    stateT        state;
    neighbor_type inputs;
    std::funciton<stateT(const timeT t, const stateT s, neighbor_type const&)>
                  reaction;
};

} // amime
#endif /* AMIME_CORE_NODE */

#ifndef AMIME_CORE_NODE
#define AMIME_CORE_NODE
#include <vector>
#include <functional>

namespace amime
{

template<typename stateT>
struct Node
{
    stateT    state;
    std::funciton<stateT(std::vector<Node const*> const&)> reaction;
    std::vector<Node const*> inputs;
};

} // amime
#endif /* AMIME_CORE_NODE */

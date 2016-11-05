#include <amime/core/Simulator.hpp>
#include <amime/core/RungeKuttaODESolver.hpp>
#include <iostream>
#include <iomanip>
#include <cassert>

typedef amime::Node<double, double> node_type;

constexpr double alpha = 200.0;
constexpr double beta  = 3.0;
constexpr double alpha_0 = 0.2;

double transcript(const double t, const double s,
                  const std::vector<const node_type*>& neighbors)
{
    const double p = neighbors.at(0)->state;
    return alpha_0 - s + (alpha / (1 + p*p));
}

double translate(const double t, const double s,
                 const std::vector<const node_type*>& neighbors)
{
    return beta * (neighbors.at(0)->state - s);
}

struct OscillatorTraits
{
    typedef double real_type;
    typedef real_type time_type;
    typedef real_type state_type;
    typedef std::size_t node_id_type;
};

int main()
{
    node_type protein1, protein2, protein3;
    node_type mRNA1, mRNA2, mRNA3;

    protein1.state = 10.0;
    protein1.reaction = translate;
    mRNA1.state = 100.0;
    mRNA1.reaction = transcript;

    protein2.state = 10.0;
    protein2.reaction = translate;
    mRNA2.state = 80.0;
    mRNA2.reaction = transcript;

    protein3.state = 10.0;
    protein3.reaction = translate;
    mRNA3.state = 50.0;
    mRNA3.reaction = transcript;

    amime::Network<OscillatorTraits> network;
    network.insert(0, protein1);
    network.insert(1, protein2);
    network.insert(2, protein3);
    network.insert(3, mRNA1);
    network.insert(4, mRNA2);
    network.insert(5, mRNA3);

    network.at(0).inputs.push_back(&(network.at(3)));
    network.at(1).inputs.push_back(&(network.at(4)));
    network.at(2).inputs.push_back(&(network.at(5)));

    network.at(3).inputs.push_back(&(network.at(1)));
    network.at(4).inputs.push_back(&(network.at(2)));
    network.at(5).inputs.push_back(&(network.at(0)));

    amime::RungeKuttaODESolver<OscillatorTraits> solver(0.01);
    amime::Simulator<OscillatorTraits, amime::RungeKuttaODESolver>
        simulator(100.0, solver, network);

    std::cout << std::setprecision(8) << std::endl;
    while(simulator.step())
    {
        amime::Network<OscillatorTraits> const& net = simulator.network();
        std::cout << simulator.time() << " " << net.at(0).state << " "
                  << net.at(1).state  << " " << net.at(2).state << " "
                  << net.at(3).state  << " " << net.at(4).state << " "
                  << net.at(5).state  << std::endl;
    }

    return 0;
}

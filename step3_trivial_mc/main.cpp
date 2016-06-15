#include <alps/mc/stop_callback.hpp>
#include "simulation.hpp"

int main(int argc, char** argv)
{
    alps::params p(argc, (const char**)argv);
    MySimulation::define_parameters(p)
        .define<int>("time", 5, "Time limit for the computation");
        
    if (p.help_requested(std::cerr) || p.has_missing(std::cerr))
        return 1;

    std::cout << "Creating simulation..." << std::endl;
    MySimulation mysim(p);

    std::cout << "Starting simulation..." << std::endl;
    mysim.run(alps::stop_callback(int(p["time"])));

    std::cout << "Simulation finished, ran for " << mysim.count() << " steps." << std::endl;
}

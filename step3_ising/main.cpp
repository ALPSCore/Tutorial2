#include "ising.hpp"
#include <iostream>
#include <alps/accumulators.hpp>
#include <alps/mc/mcbase.hpp>
#include <alps/mc/stop_callback.hpp>

int main(int argc, char* argv[])
{
    // Shortcut for alps::accumulators namespace
    namespace aa=alps::accumulators;

    // Shortcut for the type for the simulation
    typedef ising_sim my_sim_type;

    // Enclose everything in try...catch... block,
    // to gracefully report any errors.
    try {
        // Initialize parameters:
        alps::params parameters(argc, argv);
        
        // Define simulation-specific parameters...
        my_sim_type::define_parameters(parameters)
            // ...and add one more, with the default of 5.
            .define<std::size_t>("timelimit", 5, "Time limit for the computation");

        // Check if there are problems
        if (parameters.help_requested(std::cout) ||
            parameters.has_missing(std::cout)) {
            return 1;
        }
    
        std::cout << "Creating simulation..."
                  << std::endl;
        my_sim_type sim(parameters); 

        // Run the simulation
        std::cout << "Running simulation..."
                  << std::endl;
        sim.run(alps::stop_callback(size_t(parameters["timelimit"])));

        
        std::cout << "Collecting results..."
                  << std::endl;
        aa::result_set results = sim.collect_results();

        // Print results
        std::cout << "All measured results:"
                  << std::endl;
        std::cout << results << std::endl;
            
        std::cout << "Simulation ran for "
                  << results["Energy"].count()
                  << " steps." << std::endl;

        // Assign individual results to variables.
        aa::result_wrapper mag4=results["Magnetization^4"];
        aa::result_wrapper mag2=results["Magnetization^2"];

        // Derived result: Binder Cumulant
        aa::result_wrapper binder_cumulant=1-mag4/(3*mag2*mag2);
        std::cout << "Binder cumulant: " << binder_cumulant
                  << std::endl;

        
        return 0;

    } catch (const std::runtime_error& exc) {
        std::cout << "Exception caught: " << exc.what() << std::endl;
        return 2;
    } catch (...) {
        std::cout << "Unknown exception caught, re-throwing..." << std::endl;
        throw;
    }
}

#include <iostream>
#include <alps/mc/stop_callback.hpp>
#include <alps/mc/mpiadapter.hpp>
#include "simulation.hpp"

int main(int argc, char** argv)
{
    // Define shorthand for alps::accumulators namespace:
    namespace aa = alps::accumulators;
  
    // Define shorthand for our simulation class:
    typedef alps::mcmpiadapter<MySimulation> mysim_type;
    
    // init MPI, obtain communicator
    alps::mpi::environment mpi_env(argc, argv);
    alps::mpi::communicator comm;
    
    // remember the rank
    const int rank=comm.rank();
    const bool is_master=(0==rank);
    
   // Parse the parameters
    alps::params p(argc, argv, comm);

    // Define the simulation parameters...
    mysim_type::define_parameters(p)
      // ...and add one more parameter (with default value of 5):
      .define<std::size_t>("timelimit", 5, "Time limit for the computation");

    // Check if user needs help or is missing something
    if (p.help_requested(std::cerr) || p.has_missing(std::cerr))
        return 1;

    std::cout << "Creating simulation"
              << " on rank " << rank
              << std::endl;

    mysim_type mysim(p, comm);

    std::cout << "Starting simulation"
              << " on rank " << rank
              << std::endl;

    mysim.run(alps::stop_callback(std::size_t(p["timelimit"])));

    std::cout << "Simulation finished"
              << " on rank " << rank
              << std::endl
              << "Collecting results..."
              << std::endl;
    
    aa::result_set results=mysim.collect_results();

    // Do printing only on master:
    if (is_master) {
        // Print all results:
        std::cout << "All results:\n" << results << std::endl;

        // Access individual results:
        aa::result_wrapper r=results["hits"];
        std::cout << "Simulation ran for "
                  << r.count()
                  << " steps." << std::endl;

        // should get $\pi$:
        aa::result_wrapper pi_result=r*4.;

        // print the mean:
        std::cout << "Mean: " << pi_result.mean<double>() << std::endl;
    
        // print the error bar, and the range:
        std::cout << "Error: " << pi_result.error<double>() << std::endl;
        std::cout << "Range: "
                  << pi_result.mean<double>()-pi_result.error<double>()
                  << " ... "
                  << pi_result.mean<double>()+pi_result.error<double>()
                  << std::endl;
    }

    return 0;
}

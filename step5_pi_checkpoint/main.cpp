#include <iostream>
#include <alps/mc/stop_callback.hpp>
#include "simulation.hpp"

int main(int argc, char** argv)
{
    // Define shorthand:
    typedef MySimulation mysim_type;
    
    // Parse the parameters
    alps::params p(argc, (const char**)argv);

    // Define the parameters
    mysim_type::define_parameters(p)
        .define<std::size_t>("timelimit", 5, "Time limit for the computation");
        
    if (p.help_requested(std::cerr) || p.has_missing(std::cerr))
        return 1;

    std::cout << "Creating simulation"
              << std::endl;

    mysim_type mysim(p);

    // If needed, restore the last checkpoint
    std::string checkpoint_file = p["checkpoint"].as<std::string>();
        
    if (p.is_restored()) {
        std::cout << "Restoring checkpoint from " << checkpoint_file
                  << std::endl;
        mysim.load(checkpoint_file);
    }

    std::cout << "Starting simulation"
              << std::endl;

    mysim.run(alps::stop_callback(std::size_t(p["timelimit"])));

    std::cout << "Simulation finished"
              << std::endl;

    std::cout << "Saving to checkpoint " << checkpoint_file
              << std::endl;

    mysim.save(checkpoint_file);

    std::cout << "Collecting results..."
              << std::endl;
    
    alps::accumulators::result_set results=mysim.collect_results();

    // Print all results:
    std::cout << "All results:\n" << results << std::endl;

    // Access individual results:
    const alps::accumulators::result_wrapper& obj=results["objective"];
    std::cout << "Simulation ran for "
              << obj.count()
              << " steps." << std::endl;

    // should get $\pi$:
    const alps::accumulators::result_wrapper& pi_res=obj*4.;

    std::cout << "Mean: " << pi_res.mean<double>() << std::endl;
    std::cout << "Error: " << pi_res.error<double>() << std::endl;
    std::cout << "Range: "
              << pi_res.mean<double>()-pi_res.error<double>()
              << " ... "
              << pi_res.mean<double>()+pi_res.error<double>()
              << std::endl;
    std::cout << "Autocorrelation length: "
              << pi_res.autocorrelation<double>()
              << std::endl;

    // Saving to the output file
    std::string output_file = p["outputfile"];
    std::cout << "Saving results to " << output_file << std::endl;
    alps::hdf5::archive ar(output_file, "w");
    ar["/parameters"] << p;
    ar["/simulation/results"] << results;

    return 0;
}

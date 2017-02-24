#pragma once

#include <alps/mc/mcbase.hpp>
#include <alps/accumulators.hpp>

class MySimulation : public alps::mcbase {
  private:
    double x_, y_; ///< Coordinates of the random point
    long max_trials_; //< Max. number of trials (how long to run the simulation)

  public:
    /// Constructor for the simulation
    /** @param params           Input simulation parameters
        @param seed_offset      Not needed for now, ignore.
    */
    MySimulation(const parameters_type& params, std::size_t seed_offset=0);

    /// Generates a new random point
    void update();

    /// Collects statistics
    void measure();

    /// What fraction of the simulation is completed?
    /** @returns        Completed fraction of the simulation (>1.0 means we are done!) */
    double fraction_completed() const;

    /// Defines problem-specific input parameters
    static parameters_type& define_parameters(parameters_type&);


    /// Accumulator type to collect observables (for our convenience)
    typedef alps::accumulators::NoBinningAccumulator<double> my_accumulator_type;
};

#pragma once

#include <alps/mc/mcbase.hpp>
#include <alps/accumulators.hpp>

class MySimulation : public alps::mcbase {
  private:
    long burnin_;
    long maxcount_;
    double stepsize_;
    
    long istep_;
    double x_,y_;
    
  public:
    // These we need for the simulation.
    static bool is_inside_area(double x, double y);
    static double objective_function(double x, double y);

    // Accumulator type to collect observables.
    typedef alps::accumulators::FullBinningAccumulator<double> my_accumulator_type;
    
    MySimulation(const parameters_type& params, std::size_t seed_offset=0);

    void update();
    void measure();
    double fraction_completed() const;
    
    static parameters_type& define_parameters(parameters_type&);        

    using alps::mcbase::save;
    using alps::mcbase::load;
    void save(alps::hdf5::archive & ar) const;
    void load(alps::hdf5::archive & ar);
};

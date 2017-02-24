#include "simulation.hpp"
#include <cmath>

// Constructor
MySimulation::MySimulation(const parameters_type& params, std::size_t seed_offset)
    : alps::mcbase(params,seed_offset)  // pass arguments to parent class constructor
{
    // Get and remember the max. number of trials
    max_trials_=params["trials"];
    
    // Initialize slots for the measurements ("observables").
    // The `measurements` is provided by the parent class.
    measurements << my_accumulator_type("hits");
}


// Throw a point
void MySimulation::update() {
    // Generate 2 random numbers in 1x1 square around the origin:
    x_=random()-0.5;
    y_=random()-0.5;
}

// Collect statistics
void MySimulation::measure() {
    // Check if we hit the circle of radius 0.5, centered at (0,0)
    int hit=(x_*x_ + y_*y_ <= 0.5*0.5);
    measurements["hits"] << hit;
}

double MySimulation::fraction_completed() const {
    return count(measurements["hits"])/double(max_trials_);
}

MySimulation::parameters_type& MySimulation::define_parameters(MySimulation::parameters_type& params) {
    // Parameters defined by the base class:
    return alps::mcbase::define_parameters(params)
        // and by our class:
        .description("Demo Monte Carlo program")
        .define<long>("trials", "Number of trials (number of points to throw)");
}

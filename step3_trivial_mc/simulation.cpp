#include <iostream>
#include "simulation.hpp"    

MySimulation::MySimulation(const parameters_type& params, std::size_t seed_offset)
    : alps::mcbase(params, seed_offset),
          istep_(0)   {
        maxcount_=params["count"];
        verbose_=params["verbose"];
    }

void MySimulation::update() {
    double r=random();
    if (verbose_) {
        std::cout << "Update at step " << istep_ << ", random=" << r << std::endl;
    }
    ++istep_;
}

void MySimulation::measure() {
    if (verbose_) {
        std::cout << "Measure at step " << istep_ << std::endl;
    }
}

double MySimulation::fraction_completed() const {
    double frac=double(istep_)/maxcount_;
    return frac;
}

MySimulation::parameters_type& MySimulation::define_parameters(MySimulation::parameters_type& params) {
    return alps::mcbase::define_parameters(params)
        .description("Dummy Monte Carlo program")
        .define<int>("count", "Number of steps to go")
        .define("verbose", "Report steps");
}

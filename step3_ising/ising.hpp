#pragma once

#include <alps/mc/mcbase.hpp>

// This is for our storage for spins
#include "storage_type.hpp"

// Simulation class for 2D Ising model (square lattice).
// Extends alps::mcbase, the base class of all Monte Carlo simulations.
// Defines its state and calculation functions (update/measure)
class ising_sim : public alps::mcbase {
    // The internal state of our simulation
  private:
    int length_; // the same in both dimensions
    int sweeps_;
    int thermalization_sweeps_;
    int total_sweeps_;
    double beta_;
    storage_type spins_;
    double current_energy_;
    double current_magnetization_;

  public:
    /// Constructor
    ising_sim(parameters_type const & parms, std::size_t seed_offset = 0);

    /// Defines model-specific parameters
    static alps::params& define_parameters(alps::params& parameters);

    /// MC step
    void update();
    /// Measurements of quantities
    void measure();

    /// How far we are proceeded
    double fraction_completed() const;
};


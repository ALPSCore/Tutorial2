#include "ising.hpp"

// Defines the parameters for the ising simulation
alps::params& ising_sim::define_parameters(alps::params& parameters) {
    // Parameters of the base class...
    alps::mcbase::define_parameters(parameters)
    // followed by Ising specific parameters
        .description("2D ising simulation")
        .define<int>("length", "size of the periodic box")
        .define<int>("sweeps", 1000000, "maximum number of sweeps")
        .define<int>("thermalization", 10000, "number of sweeps for thermalization")
        .define<double>("temp", "temperature of the system")
        .define<std::string>("acc", "accumulators to use: nobin|logbin|fullbin");
    return parameters;
}

// Creates a new simulation.
// We always need the parameters and the seed as we need to pass it to
// the alps::mcbase constructor. We also initialize our internal state,
// mainly using values from the parameters.
ising_sim::ising_sim(parameters_type const & prm, std::size_t seed_offset)
    : alps::mcbase(prm, seed_offset)
    , length_(parameters["length"])
    , sweeps_(0)
    , thermalization_sweeps_(int(prm["thermalization"]))
    , total_sweeps_(prm["sweeps"])
    , beta_(1. / prm["temp"].as<double>())
    , spins_(length_,length_)
    , current_energy_(0)
    , current_magnetization_(0)
{

    // Add the accumulators for the values being measured;
    // the type of accumulators depends on the user choice in parameters.
    if (prm["acc"] == "fullbin") {
        measurements
            << alps::accumulators::FullBinningAccumulator<double>("Energy")
            << alps::accumulators::FullBinningAccumulator<double>("Magnetization")
            << alps::accumulators::FullBinningAccumulator<double>("AbsMagnetization")
            << alps::accumulators::FullBinningAccumulator<double>("Magnetization^2")
            << alps::accumulators::FullBinningAccumulator<double>("Magnetization^4")
            ;
    } else if (prm["acc"] == "logbin") {
        measurements
            << alps::accumulators::LogBinningAccumulator<double>("Energy")
            << alps::accumulators::LogBinningAccumulator<double>("Magnetization")
            << alps::accumulators::LogBinningAccumulator<double>("AbsMagnetization")
            << alps::accumulators::LogBinningAccumulator<double>("Magnetization^2")
            << alps::accumulators::LogBinningAccumulator<double>("Magnetization^4")
            ;
    } else if (prm["acc"] == "nobin") {
        measurements
            << alps::accumulators::NoBinningAccumulator<double>("Energy")
            << alps::accumulators::NoBinningAccumulator<double>("Magnetization")
            << alps::accumulators::NoBinningAccumulator<double>("AbsMagnetization")
            << alps::accumulators::NoBinningAccumulator<double>("Magnetization^2")
            << alps::accumulators::NoBinningAccumulator<double>("Magnetization^4")
            ;
    } else {
        // Not a valid choice, we have to throw:
        throw std::runtime_error("Invalid choice of accumulator: '"
                                 +prm["acc"].as<std::string>()
                                 +"'");
    }
    
    // Initializes the spins
    for(int i=0; i<length_; ++i) {
        for (int j=0; j<length_; ++j) {
            spins_(i,j) = (random() < 0.5 ? 1 : -1);
        }
    }

    // Calculates initial magnetization and energy
    for (int i=0; i<length_; ++i) {
        for (int j=0; j<length_; ++j) {
            current_magnetization_ += spins_(i,j);
            int i_next=(i+1)%length_; // wrap around (PBC)
            int j_next=(j+1)%length_; // wrap around (PBC)
            current_energy_ += -(spins_(i,j)*spins_(i,j_next)+
                                 spins_(i,j)*spins_(i_next,j));
            
        }
    }
    
}

// Performs the calculation at each MC step;
// decides if the step is accepted.
void ising_sim::update() {
    using std::exp;

    // Choose a spin to flip:
    int i = int(length_ * random());
    int j = int(length_ * random());

    // Find neighbors indices, with wrap over box boundaries:
    int i_nxt = (i+1) % length_;
    int i_prv = (i-1+length_) % length_;
    int j_nxt = (j+1) % length_;
    int j_prv = (j-1+length_) % length_;

    // Energy difference:
    double delta=2.*spins_(i,j)*
                    (spins_(i_nxt,j)+
                     spins_(i_prv,j)+
                     spins_(i,j_nxt)+
                     spins_(i,j_prv));
    
    // Step acceptance:
    if (delta<=0. || random() < exp(-beta_*delta)) {
        // update energy:
        current_energy_ += delta;
        // update magnetization:
        current_magnetization_ -= 2*spins_(i,j);
        // flip the spin
        spins_(i,j) = -spins_(i,j);
    }        
}

// Collects the measurements at each MC step.
void ising_sim::measure() {
    ++sweeps_;
    
    // Check if we are still thermalizing:
    if (sweeps_<thermalization_sweeps_) return;
    
    const double n=length_*length_; // number of sites
    double tmag = current_magnetization_ / n; // magnetization

    // Accumulate the data (per site)
    measurements["Energy"] << (current_energy_ / n);
    measurements["Magnetization"] << tmag;
    measurements["AbsMagnetization"] << fabs(tmag);
    measurements["Magnetization^2"] << tmag*tmag;
    measurements["Magnetization^4"] << tmag*tmag*tmag*tmag;
}

// Returns a number between 0.0 and 1.0 with the completion percentage
double ising_sim::fraction_completed() const {
    double f=0;
    if (sweeps_ >= thermalization_sweeps_) {
        f=(sweeps_-thermalization_sweeps_)/double(total_sweeps_);
    }
    return f;
}


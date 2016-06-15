#pragma once

#include <alps/mc/mcbase.hpp>

class MySimulation : public alps::mcbase {
  private:
    int istep_;
    int maxcount_;
    bool verbose_;

  public:
    MySimulation(const parameters_type& params, std::size_t seed_offset=0);

    void update();
    void measure();
    double fraction_completed() const;

    static parameters_type& define_parameters(parameters_type&);        

    int count() { return istep_; }
};

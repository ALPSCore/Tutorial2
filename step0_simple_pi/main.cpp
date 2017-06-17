#include <iostream>
#include <alps/mc/random01.hpp>
#include <alps/accumulators.hpp>
#include <alps/params.hpp>

int main(int argc, char** argv)
{
    namespace aa = alps::accumulators;
    alps::params p(argc, argv); // Parse the parameters
    p.description("Simple MC")
        .define<long>("trials", "Number of MC trials");

    if (p.help_requested(std::cerr) || p.has_missing(std::cerr))
        return 1;

    aa::accumulator_set aset;
    aset << aa::NoBinningAccumulator<double>("hits");

    alps::random01 random(42);
    for (long istep=0; istep<p["trials"]; ++istep) {
        double x=random()*2-1;
        double y=random()*2-1;
        if (x*x + y*y < 1)
            aset["hits"]<<1;
        else
            aset["hits"]<<0;
    }
    aa::result_set rset(aset);
    rset["pi"]=rset["hits"]*4.0;

    std::cout << rset;
    return 0;
}

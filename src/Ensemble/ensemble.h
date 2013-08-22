#ifndef MD_ENSEMBLE_H
#define MD_ENSEMBLE_H

#include "../defines.h"
#include "../region/region.h"

class Thermostat;

class Ensemble
{
public:
    Ensemble(Thermostat *thermostat, int nSpecies, const double *sigmas, const double *epses);

    friend void Region::append(int i);

private:

    int nSpecies;

    mat sigmaTable;
    mat epsTable;

    mat::fixed<MD_N, MD_DIM> pos;
    mat::fixed<MD_N, MD_DIM> vel;
    mat::fixed<MD_N, MD_DIM> forces;

    Thermostat *thermostat;

};

#endif // MD_ENSEMBLE_H

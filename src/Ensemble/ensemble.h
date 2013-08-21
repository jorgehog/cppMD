#ifndef ENSEMBLE_H
#define ENSEMBLE_H

#include "../defines.h"
#include "../region/region.h"

class Thermostat;

class Ensemble
{
public:
    Ensemble(Thermostat *thermostat);

    friend void Region::append(int i);

private:

    mat::fixed<N, DIM> pos;
    mat::fixed<N, DIM> vel;
    mat::fixed<N, DIM> forces;

    Thermostat *thermostat;

};

#endif // ENSEMBLE_H

#ifndef MD_ENSEMBLE_H
#define MD_ENSEMBLE_H

#include <armadillo>

using namespace arma;

#include "../defines.h"

struct Ensemble
{

    mat::fixed<ENS_DIM, ENS_N> pos;
    mat::fixed<ENS_DIM, ENS_N> vel;
    mat::fixed<ENS_DIM, ENS_N> forces;
    cube::fixed<ENS_N, ENS_N, ENS_DIM> forceVectors;

    const vec masses;

    const int nSpecies;

    Ensemble(const vec & masses) : masses(masses), nSpecies(masses.n_elem) {}

};

#endif // MD_ENSEMBLE_H

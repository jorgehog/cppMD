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
};

#endif // MD_ENSEMBLE_H

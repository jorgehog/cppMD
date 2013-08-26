#ifndef MD_ENSEMBLE_H
#define MD_ENSEMBLE_H

#include <armadillo>

using namespace arma;

#include "../defines.h"

struct Ensemble
{
    mat::fixed<ENS_N, ENS_DIM> pos;
    mat::fixed<ENS_N, ENS_DIM> vel;
    mat::fixed<ENS_N, ENS_DIM> forces;
};

#endif // MD_ENSEMBLE_H

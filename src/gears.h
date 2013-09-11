#ifndef GEARS_H
#define GEARS_H

#include <armadillo>
#include "defines.h"

using namespace arma;

namespace gears {

void cancelLinearMomentum(mat & vel)
{
    vec pTot = zeros<vec>(ENS_DIM);

    for (int k = 0; k < ENS_N; ++k) {
        pTot += vel.col(k);
    }

    pTot /= ENS_N;

    for (int i = 0; i < ENS_N; ++i) {
        for (int j = 0; j < ENS_DIM; ++j) {
            vel(j, i) -= pTot(j);
        }
    }

}

} //End namespace gears

#endif // GEARS_H

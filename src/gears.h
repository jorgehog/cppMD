#ifndef GEARS_H
#define GEARS_H

#include <armadillo>

using namespace arma;

#include "defines.h"
#include "MeshField/meshfield.h"
#include "Ensemble/ensemble.h"

namespace gears {

inline void cancelLinearMomentum(mat & vel)
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

inline double getKineticEnergy(const MeshField * mf){

    double Ek = 0;

    const int MASS = 1;

    const std::vector<int> & atoms = mf->getAtoms();
    for (const int & k : atoms) {
        Ek += mf->getEnsemble()->vel(0, k)*mf->getEnsemble()->vel(0, k)
                + mf->getEnsemble()->vel(1, k)*mf->getEnsemble()->vel(1, k);
#if ENS_DIM == 3
        Ek += mf->getEnsemble()->vel(2, k)*mf->getEnsemble()->vel(2, k);
#endif
    }

    return 0.5*MASS*Ek;

}

inline double getTemperature(MeshField * mf){

    return 2*getKineticEnergy(mf)/(3*mf->getPopulation());

}



} //End namespace gears

#endif // GEARS_H

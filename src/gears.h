#ifndef GEARS_H
#define GEARS_H

#include <armadillo>

using namespace arma;

#include "defines.h"
#include "MeshField/meshfield.h"
#include "Ensemble/ensemble.h"

namespace gears {

inline void cancelLinearMomentum(Ensemble* ensemble)
{
    vec pTot = zeros<vec>(ENS_DIM);

    //retrieve ensemble values;
    const vec & masses = ensemble->masses;
    mat & vel = ensemble->vel;
    const int & N = ensemble->nSpecies;


    //Calculates total linear momentum
    for (int k = 0; k < ENS_N; ++k) {
        pTot += masses(k%N)*vel.col(k);
    }


    pTot /= ENS_N;

    //subtract the velocity in such a way that the momentum is zero.
    for (int i = 0; i < ENS_N; ++i) {
        for (int j = 0; j < ENS_DIM; ++j) {
            vel(j, i) -= pTot(j)/masses(i%N);
        }
    }

}

inline double getKineticEnergy(const MeshField * mf){

    double vVecSquared;
    double Ek = 0;

    const Ensemble* ensemble = mf->getEnsemble();

    const vec & masses = ensemble->masses;
    const int & N = ensemble->nSpecies;

    const std::vector<int> & atoms = mf->getAtoms();

    for (const int & k : atoms) {
        vVecSquared = ensemble->vel(0, k)*ensemble->vel(0, k)
                + ensemble->vel(1, k)*ensemble->vel(1, k);
#if ENS_DIM == 3
        vVecSquared += ensemble->vel(2, k)*ensemble->vel(2, k);
#endif
        Ek += masses(k%N)*vVecSquared;
    }

    return 0.5*Ek;

}

inline double getTemperature(MeshField * mf){

    return 2*getKineticEnergy(mf)/(3*mf->getPopulation());

}



} //End namespace gears

#endif // GEARS_H

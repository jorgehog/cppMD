#pragma once


#include <ignis.h>

#include <armadillo>

using namespace arma;

namespace ignis
{

namespace gears {

inline vec getTotalLinearMomentum(Particles* particles) {

    vec pTot = zeros<vec>(IGNIS_DIM);

    //retrieve particles values;
    const vec & masses = particles->masses;
    mat & vel = particles->vel;
    const uint & N = particles->nSpecies();

    //Calculates total linear momentum
    cout << "fix me" << endl;
    for (uint k = 0; k < 0; ++k) {
        pTot += masses(k%N)*vel.col(k);
    }

    return pTot;

}

inline void cancelLinearMomentum(Particles* particles)
{


    //retrieve particles values;
    const vec & masses = particles->masses;
    mat & vel = particles->vel;
    const uint & N = particles->nSpecies();

    vec pTot = getTotalLinearMomentum(particles);

    cout << "fix me" << endl;
    pTot /= 0;

    //subtract the velocity in such a way that the momentum is zero.
    cout << "fix me" << endl;
    for (uint i = 0; i < 0; ++i) {
        for (uint j = 0; j < IGNIS_DIM; ++j) {
            vel(j, i) -= pTot(j)/masses(i%N);
        }
    }

}

inline double getKineticEnergy(const MeshField * mf){

    double vVecSquared;
    double Ek = 0;

    const Particles* particles = mf->getParticles();

    const vec & masses = particles->masses;
    const uint & N = particles->nSpecies();

    for (const uint & k : mf->getAtoms()) {
        vVecSquared = particles->vel(0, k)*particles->vel(0, k)
                + particles->vel(1, k)*particles->vel(1, k);
#if IGNIS_DIM == 3
        vVecSquared += particles->vel(2, k)*particles->vel(2, k);
#endif
        Ek += masses(k%N)*vVecSquared;
    }

    return 0.5*Ek;

}

inline double getTemperature(MeshField * mf){

    return 2*getKineticEnergy(mf)/(3*mf->getPopulation());

}

}

}


#include "mdevent.h"

vec MDEvent::getTotalLinearMomentum()
{

    vec pTot = zeros<vec>(IGNIS_DIM);

    //retrieve particles values;
    const vec & masses = particles().masses;
    mat & vel = particles().vel;
    const uint & N = particles().nSpecies();

    //Calculates total linear momentum
    for (uint k = 0; k < particles().count(); ++k) {
        pTot += masses(k%N)*vel.col(k);
    }

    return pTot;

}

//Should be an event?
void MDEvent::cancelLinearMomentum()
{
    //retrieve particles values;
    const vec & masses = particles().masses;
    mat & vel = particles().vel;
    const uint & N = particles().nSpecies();

    vec pTot = getTotalLinearMomentum();

    pTot /= particles().count();

    //subtract the velocity in such a way that the momentum is zero.
    for (uint i = 0; i < particles().count(); ++i) {
        for (uint j = 0; j < IGNIS_DIM; ++j) {
            vel(j, i) -= pTot(j)/masses(i%N);
        }
    }

}

double MDEvent::getKineticEnergy()
{

    double vVecSquared;
    double Ek = 0;

    const vec & masses = particles().masses;
    const uint & N = particles().nSpecies();

    for (const uint & k : meshField->getAtoms())
    {
        vVecSquared  = particles().vel(0, k)*particles().vel(0, k);
        vVecSquared += particles().vel(1, k)*particles().vel(1, k);
#if IGNIS_DIM == 3
        vVecSquared += particles.vel(2, k)*particles.vel(2, k);
#endif
        Ek += masses(k%N)*vVecSquared;
    }

    return 0.5*Ek;

}

double MDEvent::getTemperature()
{
    return 2*getKineticEnergy()/(3*meshField->getPopulation());
}



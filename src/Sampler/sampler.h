#ifndef SAMPLER_H
#define SAMPLER_H

#include "../MeshField/meshfield.h"


class fieldSampler {

public:

static double getKineticEnergy(MeshField * mf){
    double Ek = 0;

    const int MASS = 1;

    for (const int & k : mf->getAtoms()) {
        Ek += mf->ensemble->vel(0, k)*mf->ensemble->vel(0, k)
                + mf->ensemble->vel(1, k)*mf->ensemble->vel(1, k);
#if ENS_DIM == 3
        Ek += mf->ensemble->vel(2, k)*mf->ensemble->vel(2, k);
#endif
    }
//    std::cout << "Ek" << 0.5*Ek/ENS_N << std::endl;
//    std::cin >> Ek;
    return 0.5*MASS*Ek;

}

static double getTemperature(MeshField * mf){

    return 2*getKineticEnergy(mf)/(3*mf->getPopulation());

}

};

#endif // SAMPLER_H

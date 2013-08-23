#ifndef MD_ENSEMBLE_H
#define MD_ENSEMBLE_H

#include "../defines.h"
#include "../meshField/meshField.h"

class Ensemble
{
public:
    Ensemble(int nSpecies, const double *sigmas, const double *epses);

    friend void meshField::append(int i);

private:

    int nSpecies;

    mat sigmaTable;
    mat epsTable;

    mat::fixed<MD_N, MD_DIM> pos;
    mat::fixed<MD_N, MD_DIM> vel;
    mat::fixed<MD_N, MD_DIM> forces;

};

#endif // MD_ENSEMBLE_H

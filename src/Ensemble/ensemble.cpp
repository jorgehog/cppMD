
#include "ensemble.h"
#include <math.h>

Ensemble::Ensemble(int nSpecies, const double *sigmas, const double *epses):
    nSpecies(nSpecies)
{

    sigmaTable.set_size(nSpecies, nSpecies);
    epsTable.set_size(nSpecies, nSpecies);

    for (int i = 0; i < nSpecies; ++i) {
        for (int j = 0; j < nSpecies; ++j) {
            sigmaTable(i, j) = 0.5*(sigmas[i] + sigmas[j]);
            epsTable(i, j) = sqrt(epses[i]*epses[j]);
        }
    }

}



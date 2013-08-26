#include "mdsolver.h"

#include "../Event/predefinedEvents/predefinedevents.h"

#include "../Ensemble/ensemble.h"

#include <stdlib.h>
#include <time.h>

#include "../Event/event.h"


mdSolver::mdSolver(double dt, int N, int nSpecies, const double *sigmas, const double *epses):
    SolverEvent(dt, N),
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

    solverSpecificEvents.push_back(new randomShuffle());
    solverSpecificEvents.push_back(new periodicScaling());

}

void mdSolver::initialize()
{

    srand(time(NULL));

    ensemble->pos.randu();

    for (int i = 0; i < ENS_DIM; ++i) {
        ensemble->pos.col(i) *= mainMesh->shape(i);
    }

}

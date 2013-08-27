#include "mdsolver.h"

#include "../Event/predefinedEvents/predefinedevents.h"

#include "../Ensemble/ensemble.h"

#include <stdlib.h>
#include <time.h>

#include "../Event/event.h"


mdSolver::mdSolver(double dt, int N, int nSpecies, double T0, const double *sigmas, const double *epses):
    SolverEvent(dt, N),
    nSpecies(nSpecies),
    T0(T0),
    sqrtkT0(sqrt(datum::k*T0))
{

    sigmaTable.set_size(nSpecies, nSpecies);
    epsTable.set_size(nSpecies, nSpecies);

    for (int i = 0; i < nSpecies; ++i) {
        for (int j = 0; j < nSpecies; ++j) {
            sigmaTable(i, j) = 0.5*(sigmas[i] + sigmas[j]);
            epsTable(i, j) = sqrt(epses[i]*epses[j]);
        }
    }

    solverSpecificEvents.push_back(new VelocityVerletFirstHalf(dt));
    solverSpecificEvents.push_back(new LennardJonesForce(this, nSpecies));
    solverSpecificEvents.push_back(new periodicScaling());
    solverSpecificEvents.push_back(new VelocityVerletSecondHalf(dt));

}

void mdSolver::initialize()
{
    double dx = mainMesh->shape(0)/ENS_NX;
    double dy = mainMesh->shape(1)/ENS_NY;

#if ENS_DIM == 3
    double dz = mainMesh->shape(2)/ENS_NZ;
    for (int k = 0; k < ENS_NZ; ++k) {
#endif

        int n = -1;

        for (int i = 0; i < ENS_NX; ++i) {
            for (int j = 0; j < ENS_NY; ++j) {
                n++;

                ensemble->pos(0, n) = i*dx;
                ensemble->pos(1, n) = j*dy;


#if ENS_DIM == 3
                ensemble->pos(2, n) = k*dz;
            }
#endif

        }
    }

    ensemble->vel.randn();
    for (int i = 0; i < ENS_N; ++i) {
        for (int k = 0; k < ENS_DIM; ++k) {
            ensemble->vel(k, i) *= sqrtkT0;
        }
    }

    std::cout << "mean" << mean(ensemble->vel, 1) << std::endl;
    std::cout << "max" << max(ensemble->pos, 1) << std::endl;


}

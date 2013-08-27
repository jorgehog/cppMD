#include "calculateforces.h"
#include "../../../Ensemble/ensemble.h"
#include "../../mdsolver.h"

TwoBodyForce::TwoBodyForce(mdSolver *solver, int nSpecies):
    Event("LJ", "", false),
    nSpecies(nSpecies),
    solver(solver)
{

}

void TwoBodyForce::execute()
{

    ensemble->forces.zeros();

    for (int i = 0; i < ENS_N; ++i) {
        for (int j = i+1; j < ENS_N; ++j) {
            solver->mainMesh->leastDistance(rRel, rRel2, i, j);
            force = getForce(i, j);

            for (int k = 0; k < ENS_DIM; ++k) {
                ensemble->forces(k, i) += force(k);
                ensemble->forces(k, j) -= force(k);
            }
        }
    }

}


LennardJonesForce::LennardJonesForce(mdSolver *solver, int nSpecies):
    TwoBodyForce(solver, nSpecies),
    mdsolver(solver)
{

}

const vec LennardJonesForce::getForce(const int &i, const int &j)
{

    int k = i%nSpecies;
    int l = j%nSpecies;

    double & sigma = mdsolver->sigmaTable(k, l);

    double & eps = mdsolver->epsTable(k, l);

    double length = sqrt(rRel2);

    double sigmaOverR6 = pow(sigma*sigma/rRel2, 3);

    force = 4*eps*(sigmaOverR6*sigmaOverR6 - sigmaOverR6)*rRel/length;

    return force;

}

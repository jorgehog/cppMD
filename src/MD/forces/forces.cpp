#include "forces.h"
#include "../../Ensemble/ensemble.h"
#include "../../MeshField/meshfield.h"

Force::Force() :
    Event("Force")
{
    ldi(0) = -1;
    ldi(1) = 0;
    ldi(2) = 1;
}


void Force::leastDistance(vec &leastRel, double &leastRel2, const int &i, const int &j)
{
    //if we have no periodicity, we are done.
#if !defined (ENS_PERIODIC_X) && !defined (ENS_PERIODIC_Y) && !defined (ENS_PERIODIC_Z)

    leastRel = ensemble->pos.col(i) - ensemble->pos.col(j);
    leastRel2 = 0;
    for (int k = 0; k < ENS_DIM; ++k) {
        leastRel2 += leastRel(k)*leastRel(k);
    }

#else

    leastRel2 = 1E23;
    vec orig(2);

    for (int c0 = 0; c0 < 3; ++c0) {
        for (int c1 = 0; c1 < 3; ++c1) {
            orig(0) = ensemble->pos(0, i) + ldi(c0)*meshField->shape(0);
            orig(1) = ensemble->pos(1, i) + ldi(c1)*meshField->shape(1);

            vec leastRelTest = orig - ensemble->pos.col(j);
            double leastRel2Test = 0;
            for (int k = 0; k < ENS_DIM; ++k) {
                leastRel2Test += leastRelTest(k)*leastRelTest(k);
            }

            if (leastRel2Test < leastRel2) {
                leastRel2 = leastRel2Test;
                leastRel = leastRelTest;
            }

        }
    }

    //Can be optimized alot

#endif

    //TODO PARTIAL PERIODICITY
}


void TwoBodyForce::execute()
{

    ensemble->forces.zeros();

    for (int i = 0; i < ENS_N; ++i) {
        for (int j = i+1; j < ENS_N; ++j) {
            leastDistance(rRel, rRel2, i, j);
            force = getForce(i, j);

            for (int k = 0; k < ENS_DIM; ++k) {
                ensemble->forces(k, i) += force(k);
                ensemble->forces(k, j) -= force(k);
            }
        }
    }
}


LennardJonesForce::LennardJonesForce(mat &sigmaTable, mat &epsTable):
    TwoBodyForce(),
    nSpecies(sigmaTable.n_cols),
    sigmaTable(sigmaTable),
    epsTable(epsTable)
{

}

const vec LennardJonesForce::getForce(const int &i, const int &j)
{

    int k = i%nSpecies;
    int l = j%nSpecies;

    double & sigma = sigmaTable(k, l);

    double & eps = epsTable(k, l);

    double length = sqrt(rRel2);

    double sigmaOverR6 = pow(sigma*sigma/rRel2, 3);

    force = 4*eps*(sigmaOverR6*sigmaOverR6 - sigmaOverR6)*rRel/length;

    return force;

}



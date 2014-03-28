#include "forces.h"

Force::Force() :
    MDEvent("Force")
{
    ldi(0) = -1;
    ldi(1) = 0;
    ldi(2) = 1;
}


void Force::leastDistance(vec &leastRel, double &leastRel2, const int &i, const int &j)
{
    //if we have no periodicity, we are done.
#if !defined (IGNIS_PERIODIC_X) && !defined (IGNIS_PERIODIC_Y) && !defined (IGNIS_PERIODIC_Z)

    leastRel = MDEvent::particles().pos.col(i) - MDEvent::particles().pos.col(j);
    leastRel2 = 0;
    for (int k = 0; k < IGNIS_DIM; ++k) {
        leastRel2 += leastRel(k)*leastRel(k);
    }

#else

    leastRel2 = 1E23;
    vec orig(2);

    for (int c0 = 0; c0 < 3; ++c0) {
        for (int c1 = 0; c1 < 3; ++c1) {
            orig(0) = particles.pos(0, i) + ldi(c0)*meshField->shape(0);
            orig(1) = particles.pos(1, i) + ldi(c1)*meshField->shape(1);

            vec leastRelTest = orig - particles.pos.col(j);
            double leastRel2Test = 0;
            for (int k = 0; k < IGNIS_DIM; ++k) {
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

    particles().forces.zeros();

    for (uint i = 0; i < particles().count(); ++i) {
        for (uint j = i+1; j < particles().count(); ++j) {
            leastDistance(rRel, rRel2, i, j);
            force = getForce(i, j);

            for (uint k = 0; k < IGNIS_DIM; ++k) {
                particles().forces(k, i) += force(k);
                particles().forces(k, j) -= force(k);

                particles().forceVectors(i, j, k) = force(k);
                particles().forceVectors(j, i, k) = -force(k);
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

    //    double length = sqrt(rRel2);

    double sigmaOverR2 = sigma*sigma/rRel2;
    double sigmaOverR6 = sigmaOverR2*sigmaOverR2*sigmaOverR2;

    force = 48*eps*sigmaOverR2*sigmaOverR6*(sigmaOverR6 - 0.5)*rRel;

    //    force = 4*eps*(sigmaOverR6*sigmaOverR6 - sigmaOverR6)*rRel/length;

    return force;

}



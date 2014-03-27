#include "mdsolver.h"

#include <armadillo>
using namespace arma;

mdSolver::mdSolver(double T0, double dt):
    Event("MDSolver"),
    sqrtkT0(sqrt(T0)),
    dt(dt)
{

}

void mdSolver::initialize()
{

    double dx = meshField->shape(0)/particles->NX;
    double dy = meshField->shape(1)/particles->NY;

#if IGNIS_DIM == 3
    double dz = meshField->shape(2)/particles->NZ;
    for (int k = 0; k < particles->NZ; ++k) {
#endif

        int n = -1;
        for (int i = 0; i < particles->NX; ++i) {
            for (int j = 0; j < particles->NY; ++j) {
                n++;

                particles->pos(0, n) = (i + 0.5*(j % 2))*dx;
                particles->pos(1, n) = j*dy;

#if IGNIS_DIM == 3
                particles->pos(2, n) = k*dz;
            }
#endif

        }
    }

    double max = 1.0;

    particles->vel.randn();

    for (int i = 0; i < particles->count(); ++i) {
        for (int k = 0; k < IGNIS_DIM; ++k) {

            particles->vel(k, i) *= sqrtkT0;

            if (particles->vel(k, i) > max) {
                particles->vel(k, i) = max;
            } else if (particles->vel(k, i) < -max ){
                particles->vel(k, i) = -max;
            }

        }
    }

    gears::cancelLinearMomentum(particles);

}

#include "mdsolver.h"

#include <armadillo>
using namespace arma;

mdSolver::mdSolver(const uvec &N, double T0, double dt):
    Event("MDSolver"),
    N(N),
    sqrtkT0(sqrt(T0)),
    dt(dt)
{

}

void mdSolver::initialize()
{

    double dx = meshField->shape(0)/N(0);
    double dy = meshField->shape(1)/N(1);

#if IGNIS_DIM == 3
    double dz = meshField->shape(2)/N(2);
    for (int k = 0; k < N(2); ++k) {
#endif

        int n = -1;
        for (int i = 0; i < N(0); ++i) {
            for (int j = 0; j < N(1); ++j) {
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
    cout << "fixme" << endl;
    for (int i = 0; i < 0; ++i) {
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

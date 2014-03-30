#include "mdsolver.h"

#include <armadillo>
using namespace arma;

mdSolver::mdSolver(double T0, double dt):
    MDEvent("MDSolver"),
    sqrtkT0(sqrt(T0)),
    dt(dt)
{

}

void mdSolver::initialize()
{

    double dx = meshField->shape(0)/particles().NX();
    double dy = meshField->shape(1)/particles().NY();

#if IGNIS_DIM == 3
    double dz = meshField->shape(2)/particles.NZ();
    for (int k = 0; k < particles.NZ(); ++k) {
#endif

        int n = -1;
        for (uint i = 0; i < particles().NX(); ++i) {
            for (uint j = 0; j < particles().NY(); ++j) {
                n++;

                particles()(n, 0) = (i + 0.5*(j % 2))*dx;
                particles()(n, 1) = j*dy;

#if IGNIS_DIM == 3
                particles(.pos))(n, 2) = k*dz;
            }
#endif

        }
    }

    double max = 1.0;

    particles().vel.randn();

    for (uint i = 0; i < particles().count(); ++i) {
        for (uint k = 0; k < IGNIS_DIM; ++k) {

            particles().vel(k, i) *= sqrtkT0;

            if (particles().vel(k, i) > max) {
                particles().vel(k, i) = max;
            } else if (particles().vel(k, i) < -max ){
                particles().vel(k, i) = -max;
            }

        }
    }

    cancelLinearMomentum();

}

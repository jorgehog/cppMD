#include "mdsolver.h"

#include "../MeshField/MainMesh/mainmesh.h"
#include "../Event/predefinedEvents/predefinedevents.h"
#include "../gears.h"
#include <armadillo>
using namespace arma;

void mdSolver::initialize()
{

//    int KS = 0;
//    int sad;

//    SaveToFile s(1);
//    s.setEnsemble(ensemble);
//    s.setLoopCyclePtr(&KS);
//    s.setMeshField(meshField);

    double dx = meshField->shape(0)/ENS_NX;
    double dy = meshField->shape(1)/ENS_NY;

#if ENS_DIM == 3
    double dz = mainMesh->shape(2)/ENS_NZ;
    for (int k = 0; k < ENS_NZ; ++k) {
#endif

        int n = -1;
        for (int i = 0; i < ENS_NX; ++i) {
            for (int j = 0; j < ENS_NY; ++j) {
                n++;

                ensemble->pos(0, n) = (i + 0.5*(j % 2))*dx;
                ensemble->pos(1, n) = j*dy;

//                s.execute();

#if ENS_DIM == 3
                ensemble->pos(2, n) = k*dz;
            }
#endif

        }
    }

    double max = 1.0;

    ensemble->vel.randn();
    for (int i = 0; i < ENS_N; ++i) {
        for (int k = 0; k < ENS_DIM; ++k) {

            ensemble->vel(k, i) *= sqrtkT0;

            if (ensemble->vel(k, i) > max) {
                ensemble->vel(k, i) = max;
            } else if (ensemble->vel(k, i) < -max ){
                ensemble->vel(k, i) = -max;
            }

        }
    }

    gears::cancelLinearMomentum(ensemble);

}

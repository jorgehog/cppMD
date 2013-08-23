#include <iostream>
#include <libconfig.h++>
#include <armadillo>

using namespace arma;
using namespace libconfig;

#include "src/Solver/solver.h"
#include "src/Ensemble/ensemble.h"
#include "src/MeshField/meshfield.h"

int main()
{

    Config cfg;
    cfg.readFile("../MD/configMD.cfg");



    solver a(10, NULL, NULL);

    const Setting & root = cfg.getRoot();

    int nSpecies = root["ensembleParameters"]["nSpecies"];

    double *sigmas = new double[nSpecies];
    double *epses = new double[nSpecies];

    for (int i = 0; i < nSpecies; ++i) {
        sigmas[i] = root["ensembleParameters"]["sigmas"][i];
        epses[i] = root["ensembleParameters"]["epses"][i];
    }

    Ensemble e(nSpecies, sigmas, epses);

    mat A(2, 2);
    A << 0 << 1 << endr << 0 << 1;
    MeshField M(A, &e);

    A.reset();
    A << 0 << 0.5 << endr << 0.5 << 1;

    MeshField M2(A, &e);

    M.addSubField(&M2);

    return 0;
}

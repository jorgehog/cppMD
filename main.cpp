#include <iostream>
#include <libconfig.h++>

using namespace libconfig;

#include "src/solver/solver.h"
#include "src/Ensemble/ensemble.h"


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

    Ensemble e(NULL, nSpecies, sigmas, epses);

    return 0;
}

#include <iostream>
#include <libconfig.h++>
#include <armadillo>

using namespace arma;
using namespace libconfig;

#include "src/defines.h"

#include "src/Ensemble/ensemble.h"
#include "src/MeshField/meshfield.h"
#include "src/MeshField/MainMesh/mainmesh.h"

#include "src/Event/event.h"
#include "src/Event/predefinedEvents/predefinedevents.h"
#include "src/Event/SolverEvent/solverevent.h"
#include "src/MD/mdsolver.h"



int main()
{

    srand(time(NULL));
    wall_clock timer;


    Config cfg;
    cfg.readFile("../MD/configMD.cfg");

    const Setting & root = cfg.getRoot();

    int nSpecies = root["ensembleParameters"]["nSpecies"];

    double *sigmas = new double[nSpecies];
    double *epses = new double[nSpecies];

    for (int i = 0; i < nSpecies; ++i) {
        sigmas[i] = root["ensembleParameters"]["sigmas"][i];
        epses[i] = root["ensembleParameters"]["epses"][i];
    }

    Ensemble e;

    countAtoms event;
    countAtoms event2;
    countAtoms event3;
    countAtoms event4;
    countAtoms event5;


    mat topology(2, 2);
    topology << 0 << 10 << endr << 0 << 10;

    mdSolver solver(0.01, 1000, nSpecies, sigmas, epses);

    MainMesh M(topology, e, solver);

    topology.reset();
    topology << 0.1 << 0.9 << endr << 0.1 << 0.9;



    MeshField M2(topology, e, "M2:M");

    topology.reset();
    topology << 0.2 << 0.8 << endr << 0.2 << 0.8;

    MeshField M3(topology, e, "M3:2");


    topology.reset();
    topology << 0.3 << 0.7 << endr << 0.3 << 0.7;

    MeshField M4(topology, e, "M4:3");

    topology.reset();
    topology << 0.2 << 0.3 << endr << 0.2 << 0.8;

    MeshField M5(topology, e, "M5:3");


    M.addSubField(M2);
    M2.addSubField(M3);
    M3.addSubField(M5);
    M3.addSubField(M4);

    M.addEvent(event);
    M2.addEvent(event2);
    M3.addEvent(event3);
    M4.addEvent(event4);
    M5.addEvent(event5);

    timer.tic();
    solver.execute();
    std::cout << "Execution time: " << timer.toc() << std::endl;

    return 0;
}

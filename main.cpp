#include <iostream>
#include <libconfig.h++>
#include <DCViz.h>
#include <armadillo>

using namespace arma;
using namespace libconfig;

#include <mpi.h>
#include "src/defines.h"

#include "src/Ensemble/ensemble.h"
#include "src/MeshField/meshfield.h"
#include "src/MeshField/MainMesh/mainmesh.h"

#include "src/Event/event.h"
#include "src/Event/predefinedEvents/predefinedevents.h"
#include "src/MD/mdsolver.h"

#include "src/MD/forces/forces.h"


int main()
{

    srand(time(NULL));
    wall_clock timer;

    Config cfg;
    cfg.readFile("../MD/configMD.cfg");

    const Setting & root = cfg.getRoot();

    /*
     * Parsing config file
     */

    double dt = root["ensembleParameters"]["dt"];
    int N = root["ensembleParameters"]["N"];

    double T0 = root["mainThermostat"]["bathTemperature"];
    double tau = ((double)root["mainThermostat"]["tauOverDt"])*dt;


    int nSpecies = root["ensembleParameters"]["nSpecies"];
    mat sigmaTable(nSpecies, nSpecies);
    mat epsTable(nSpecies, nSpecies);

    for (int i = 0; i < nSpecies; ++i) {
        for (int j = 0; j < nSpecies; ++j) {
            sigmaTable(i, j) = 0.5*((double)root["ensembleParameters"]["sigmas"][i] +
                                    (double)root["ensembleParameters"]["sigmas"][j]);

            epsTable(i, j)   = sqrt((double)root["ensembleParameters"]["epses"][i]*
                                    (double)root["ensembleParameters"]["epses"][j]);
        }
    }


    /*
     * Launching DCViz
     */

    double delay = root["DCViz"]["delay"];

    DCViz viz("/home/jorgehog/tmp/mdPos0.arma");
    viz.launch(true, delay);

    /*
     * Creating the main mesh
     */

    Ensemble e;

    mat topology(2, 2);
    topology << 0 << ENS_NX*0.8 << endr << 0 << ENS_NY*0.8;
    std::cout << topology << std::endl;

    MainMesh mainMesh(topology, e);


    /*
     * Creating events
     */

    mdSolver solver(dt, N, T0);

    VelocityVerletFirstHalf  VV1(dt);
    LennardJonesForce        F(sigmaTable, epsTable);
    periodicScaling          PBounds;
    VelocityVerletSecondHalf VV2(dt);
    BerendsenThermostat      Thermostat(T0, tau, dt);

    /*
     * Adding events
     */

    mainMesh.addSolverEvent(solver);

    //NB: The order _matters_
    mainMesh.addEvent(VV1);
    mainMesh.addEvent(F);
    mainMesh.addEvent(PBounds);
    mainMesh.addEvent(VV2);
    mainMesh.addEvent(Thermostat);

    /*
     * Lauching the solver
     */

    timer.tic();
    mainMesh.eventLoop();
    std::cout << "Execution time: " << timer.toc() << std::endl;

    return 0;
}

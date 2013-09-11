#include <iostream>
#include <libconfig.h++>
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

    double dt = root["solver"]["dt"];
    int N = root["solver"]["N"];
    int therm = root["solver"]["therm"];

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


    double delay = root["DCViz"]["delay"];



    /*
     * Creating the main mesh
     */

    Ensemble ensemble;

    double m = max(max(sigmaTable));
    double Lx = ENS_NX*m;
    double Ly = ENS_NY*m;

    mat topology(2, 2);
    topology << 0 << Lx << endr << 0 << Ly;

    MainMesh mainMesh(topology, ensemble);


    /*
     * Creating and adding events to the MainMesh
     */

    mdSolver molecularDynamicsSolver(T0, dt);
    mainMesh.addEvent(molecularDynamicsSolver);

    VelocityVerletFirstHalf  VelocityVerlet1(dt);
    mainMesh.addEvent(VelocityVerlet1);

    LennardJonesForce lennardJonesForce(sigmaTable, epsTable);
    mainMesh.addEvent(lennardJonesForce);

    periodicScaling periodicBoundaries;
    mainMesh.addEvent(periodicBoundaries);

    VelocityVerletSecondHalf VelocityVerlet2(dt);
    mainMesh.addEvent(VelocityVerlet2);

    BerendsenThermostat mainThermostat(T0, tau, dt);
    mainThermostat.setOnsetTime(0);
    mainThermostat.setOffsetTime(therm);
    mainMesh.addEvent(mainThermostat);

    ReportProgress progressReport;
    mainMesh.addEvent(progressReport);

    SaveToFile saveToFile(10);
    saveToFile.setOnsetTime(therm);
    mainMesh.addEvent(saveToFile);

    AddPressure addPressure(0.9, 0);
    addPressure.setTrigger(500);
    mainMesh.addEvent(addPressure);

    LauchDCViz launchDCViz(delay);
    launchDCViz.setTrigger(therm);
    mainMesh.addEvent(launchDCViz);

    /*
     * Creating and adding three subFields on the solver, each with their own thermostat.
     */


    mat topologyUpper (2, 2);
    mat topologyMiddle(2, 2);
    mat topologyLower (2, 2);

    topologyUpper  << 0 << Lx << endr <<    0    << 0.25*Ly;
    topologyMiddle << 0 << Lx << endr <<  0.4*Ly <<  0.6*Ly;
    topologyLower  << 0 << Lx << endr << 0.75*Ly <<      Ly;

    MeshField subFieldUpper (topologyUpper , ensemble, "heatUpper");
    MeshField subFieldMiddle(topologyMiddle, ensemble, "coolMiddle");
    MeshField subFieldLower (topologyLower , ensemble, "heatLower");

    double tTop = T0*2;
    double tMid = T0/5;
    double tLow = T0*2;

    BerendsenThermostat thermoUpper (tTop, tau, dt);
    BerendsenThermostat thermoMiddle(tMid, tau, dt);
    BerendsenThermostat thermoLower (tLow, tau, dt);

    thermoUpper.setOnsetTime (therm);
    thermoMiddle.setOnsetTime(therm);
    thermoLower.setOnsetTime (therm);

    subFieldUpper.addEvent (thermoUpper);
    subFieldMiddle.addEvent(thermoMiddle);
    subFieldLower.addEvent (thermoLower);

    mainMesh.addSubField(subFieldUpper);
    mainMesh.addSubField(subFieldMiddle);
    mainMesh.addSubField(subFieldLower);

    /*
     * Lauching the solver
     */

    timer.tic();
    mainMesh.eventLoop(N);
    std::cout << "Execution time: " << timer.toc() << std::endl;

    return 0;
}

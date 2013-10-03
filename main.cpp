#include <iostream>
#include <libconfig.h++>
#include <armadillo>

using namespace arma;
using namespace libconfig;

#include <mpi.h>
#include "cppMD.h"

int main()
{

    srand(time(NULL));
    wall_clock timer;

    Config cfg;
    cfg.readFile("../../cppMD/configMD.cfg");

    const Setting & root = cfg.getRoot();

    /*
     * Parsing config file
     */

    double dt = root["solver"]["dt"];
    int N = root["solver"]["N"];
    int therm = root["solver"]["therm"];
    double m = root["solver"]["tightness"];

    double T0 = root["mainThermostat"]["bathTemperature"];
    double tau = ((double)root["mainThermostat"]["tauOverDt"])*dt;


    int nSpecies = root["ensembleParameters"]["nSpecies"];

    mat sigmaTable(nSpecies, nSpecies);
    mat epsTable(nSpecies, nSpecies);
    vec masses(nSpecies);

    for (int i = 0; i < nSpecies; ++i) {

        masses(i) = (double)root["ensembleParameters"]["masses"][i];

        for (int j = 0; j < nSpecies; ++j) {
            sigmaTable(i, j) = 0.5*((double)root["ensembleParameters"]["sigmas"][i] +
                                    (double)root["ensembleParameters"]["sigmas"][j]);

            epsTable(i, j)   = sqrt((double)root["ensembleParameters"]["epses"][i]*
                                    (double)root["ensembleParameters"]["epses"][j]);
        }
    }

    assert(sigmaTable(0,0) == 1. && "We are working in reduced units.");
    assert(epsTable(0,0) == 1. && "We are working in reduced units.");
    assert(masses(0) == 1. && "We are working in reduced units.");


    double delay = root["DCViz"]["delay"];

    int pTime = (int)root["Events"]["Pressure"]["pTimeMinusTherm"] + therm;
    int expTime = (int)root["Events"]["Expansion"]["timeMinusPtime"] + pTime;
    int direction = (int)root["Events"]["Pressure"]["direction"];

    double delta = 1.0 - (double) root["Events"]["Pressure"]["instaStrain"];
    double tScale = (double) root["Events"]["Thermostats"]["temperatureScaleFactor"];


    /*
     * Creating the main mesh
     */

    Ensemble ensemble(masses);

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

    SaveToFile saveToFile(1);
    mainMesh.addEvent(saveToFile);

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

    AddPressure addPressure(delta, direction);
    addPressure.setTrigger(pTime);
    mainMesh.addEvent(addPressure);

    ExpandMesh expansion(1.0/delta, direction);
    expansion.setTrigger(expTime);
    mainMesh.addEvent(expansion);

//    LauchDCViz launchDCViz(delay);
//    mainMesh.addEvent(launchDCViz);

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

    double tTop = T0*tScale;
    double tMid = T0/tScale;
    double tLow = T0*tScale;

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

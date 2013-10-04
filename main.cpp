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
    cfg.readFile("../MD/configMD.cfg");

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

    int contractionTime =        (int)root["Events"]["Contraction"]["timeMinusTherm"] + therm;
    int contractionDirection =   (int)root["Events"]["Contraction"]["direction"];
    int contractionLength =      (int)root["Events"]["Contraction"]["length"];
    double contractionDelta = (double)root["Events"]["Contraction"]["delta"];

    int expansionTime =        (int)root["Events"]["Expansion"]["timeMinusContractionTime"] + contractionTime;
    int expansionDirection =   (int)root["Events"]["Expansion"]["direction"];
    int expansionLength =     ((int)root["Events"]["Expansion"]["lengthOverContractionLength"])*contractionLength;
    double expansionDelta = (double)root["Events"]["Expansion"]["delta"];

    double tScaleWarm = (double) root["Events"]["Thermostats"]["temperatureScaleFactorWarm"];
    double tScaleCold = (double) root["Events"]["Thermostats"]["temperatureScaleFactorCold"];

    double tWidth = (double) root["Events"]["Thermostats"]["widthFactor"];

    double initialDelta = (double) root["solver"]["initialDelta"];

    /*
     * Creating the main mesh
     */

    Ensemble ensemble(masses);

    double Lx = ENS_NX*m;
    double Ly = ENS_NY*m;

    mat topology(2, 2);
    topology << 0 << Lx << endr << 0 << Ly;

    MainMesh mainMesh(topology, ensemble);


    int therm10 = therm/10;
    ExpandMesh     initialExpansionX(initialDelta,    0, true, 2*therm10-1, 3*therm10-1);
    ExpandMesh     initialExpansionY(initialDelta,    1, true, 2*therm10-1, 3*therm10-1);
    ContractMesh initialContractionX(1./(initialDelta*m), 0, 5*therm10, 8*therm10);
    ContractMesh initialContractionY(1./(initialDelta*m), 1, 5*therm10, 8*therm10);


    mainMesh.addEvent(initialContractionX);
    mainMesh.addEvent(initialContractionY);
    mainMesh.addEvent(initialExpansionX);
    mainMesh.addEvent(initialExpansionY);

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

    LauchDCViz launchDCViz(delay);
    mainMesh.addEvent(launchDCViz);


    /*
     *  Onset Events
     */

    ContractMesh contraction(contractionDelta, contractionDirection, contractionTime, contractionTime + contractionLength);
    mainMesh.addEvent(contraction);

    ExpandMesh expansion(expansionDelta, expansionDirection, false, expansionTime, expansionTime + expansionLength);
    mainMesh.addEvent(expansion);


    /*
     * Creating and adding three subFields on the solver, each with their own thermostat.
     */

    mat topologyUpper (2, 2);
    mat topologyMiddle(2, 2);
    mat topologyLower (2, 2);

    topologyLower  << 0 << Lx << endr <<  0                 <<        tWidth*Ly;
    topologyMiddle << 0 << Lx << endr <<  (1 - tWidth)*Ly/2 <<  (1 + tWidth)*Ly/2;
    topologyUpper  << 0 << Lx << endr <<  (1 - tWidth)*Ly   <<               Ly;

    MeshField subFieldUpper (topologyUpper , ensemble, "heatUpper");
    MeshField subFieldMiddle(topologyMiddle, ensemble, "coolMiddle");
    MeshField subFieldLower (topologyLower , ensemble, "heatLower");

//    //DEBUG
//    debugSubMeshResize debugMeshSize1(&mainMesh);

//    debugSubMeshResize debugMeshSize2(&mainMesh);

//    debugSubMeshResize debugMeshSize3(&mainMesh);

//    subFieldUpper.addEvent(debugMeshSize1);
//    subFieldMiddle.addEvent(debugMeshSize2);
//    subFieldLower.addEvent(debugMeshSize3);

    double tTop = T0*tScaleWarm;
    double tMid = T0/tScaleCold;
    double tLow = T0*tScaleWarm;

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

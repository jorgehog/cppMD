#include <iostream>
#include <libconfig.h++>
#include <armadillo>

using namespace arma;
using namespace libconfig;

#include <mpi.h>
#include "cppMD.h"

int main(int argc, char* argv[])
{

    srand(time(NULL));
    wall_clock timer;

    Config cfg;

    std::string superPath;
    if (argc > 1) {
        superPath = std::string(argv[1]);
    } else {
        superPath = "./";
    }

    std::string configPath = superPath + "configMD.cfg";
    cfg.readFile(configPath.c_str());

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

    checkEnergyConservation ekTest;
    checkMomentumConservation pTest;
    mainMesh.addEvent(ekTest);
    mainMesh.addEvent(pTest);


#ifndef NO_DCVIZ
    LauchDCViz launchDCViz(delay);
    mainMesh.addEvent(launchDCViz);
#else
    (void) delay;
    QApplication app(argc, argv);

    QtPlatform platform(argc, argv, NULL);

    platform.setEnsemble(&ensemble);
    platform.setMainMesh(&mainMesh);

    QGraphicsView view;
    view.setObjectName(QStringLiteral("graphicsView"));
    view.setGeometry(QRect(0, 0, 600, 600));
    platform.setGraphicsView(&view);

    platform.setAdvanceTimerInterval(1./60);
    platform.exec();
#endif

    /*
     *  Onset Events
     */

    ContractMesh contraction(contractionDelta, contractionDirection, contractionTime, contractionTime + contractionLength);
    mainMesh.addEvent(contraction);

    ExpandMesh expansion(expansionDelta, expansionDirection, true, expansionTime, expansionTime + expansionLength);
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

//    density d1;
//    density d2;
//    density d3;
//    subFieldUpper.addEvent(d1);
//    subFieldMiddle.addEvent(d2);
//    subFieldLower.addEvent(d3);

    mat topologyPressureTop(2, 2);
    mat topologyPressureBottom(2, 2);

    double pressureBoxHeight = 1;

    topologyPressureTop << 0
                        << Lx
                        << endr
                        << topologyMiddle(1, 1) - pressureBoxHeight/2
                        << topologyMiddle(1, 1) + pressureBoxHeight/2;

    topologyPressureBottom << 0
                           << Lx
                           << endr
                           << topologyMiddle(1, 0) - pressureBoxHeight/2
                           << topologyMiddle(1, 0) + pressureBoxHeight/2;

    MeshField solidToLiquidTop(topologyPressureTop, ensemble, "pressureTop");
    MeshField solidToLiquidBottom(topologyPressureBottom, ensemble, "pressureBottom");

    pressureMOP pT(1);
    pressureMOP pB(1);

    solidToLiquidTop.addEvent(pT);
    solidToLiquidBottom.addEvent(pB);

    mainMesh.addSubField(solidToLiquidBottom);
    mainMesh.addSubField(solidToLiquidTop);

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
#ifndef NO_DCVIZ
    mainMesh.eventLoop(N);
#else
//    stall smoothify(1./60);
//    mainMesh.addEvent(smoothify);
    platform.startAdvanceTimer();
    MainWindow::startMDThread(N, &platform, &mainMesh);
#endif
    std::cout << "Execution time: " << timer.toc() << std::endl;

#ifndef NO_DCVIZ
    return 0;
#else
    return app.exec();
#endif
}

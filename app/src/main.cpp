#include <iostream>
#include <libconfig.h++>
#include <ignis.h>
#include <armadillo>

using namespace arma;
using namespace libconfig;

#ifdef USE_MPI
#include <mpi.h>
#endif

#include "cppMD.h"

int main(int argc, char* argv[])
{

    srand(time(NULL));
    wall_clock timer;

    Config cfg;

    std::string configPath = "../infiles/configMD.cfg";
    cfg.readFile(configPath.c_str());

    const Setting & root = cfg.getRoot();

    /*
     * Parsing config file
     */

    double dt = root["solver"]["dt"];
    int N = root["solver"]["N"];
    int therm = root["solver"]["therm"];
    double m = root["solver"]["tightness"];
    std::string outputPath = root["solver"]["outputPath"];


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

    int compressionTime =        (int)root["Events"]["Compression"]["timeMinusTherm"] + therm;
    int compressionLength =      (int)root["Events"]["Compression"]["length"];
    double compressionDelta = (double)root["Events"]["Compression"]["delta"];

    int expansionTime =        (int)root["Events"]["Expansion"]["timeMinusCompressionTime"] + compressionTime;
    int expansionLength =     ((int)root["Events"]["Expansion"]["lengthOverCompressionLength"])*compressionLength;
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
    mainMesh.setOutputPath(outputPath);

    int therm10 = therm/10;
    VolumeChange expansion(initialDelta, true);
    expansion.setOnsetTime(2*therm10-1);
    expansion.setOffsetTime(3*therm10-1);

    VolumeChange compression(1./(initialDelta*m), true);
    compression.setOnsetTime(5*therm10);
    compression.setOffsetTime(8*therm10);

    mainMesh.addEvent(expansion);
    mainMesh.addEvent(compression);

    /*
     * Creating and adding events to the MainMesh
     */

    mdSolver molecularDynamicsSolver(T0, dt);
    mainMesh.addEvent(molecularDynamicsSolver);

#ifndef NO_DCVIZ
    SaveToFile saveToFile(1);
    mainMesh.addEvent(saveToFile);
#endif

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

    temperatureFluctuations mainFluctuations(&mainThermostat);
    mainMesh.addEvent(mainFluctuations);

    ReportProgress progressReport;
    mainMesh.addEvent(progressReport);

    checkEnergyConservation ekTest;
    mainMesh.addEvent(ekTest);

    checkMomentumConservation pTest;
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

    VolumeChange compression2(compressionDelta, true);
    compression2.setOnsetTime(compressionTime);
    compression2.setOffsetTime(compressionTime + compressionLength);
    mainMesh.addEvent(compression2);

    VolumeChange expansion2(expansionDelta, true);
    expansion2.setOnsetTime(expansionTime);
    expansion2.setOffsetTime(expansionTime + expansionLength);
    mainMesh.addEvent(expansion2);


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

    density d1;
    density d2;
    density d3;

    subFieldUpper.addEvent(d1);
    subFieldMiddle.addEvent(d2);
    subFieldLower.addEvent(d3);

    pressureMOP press(1);
    subFieldMiddle.addEvent(press);

    mat topologyPressureTop(2, 2);
    mat topologyPressureBottom(2, 2);

    double pressureBoxHeight = 2.5;

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

    diffusionConstant DUpper(dt);
    diffusionConstant DMiddle(dt);
    diffusionConstant DLower(dt);

    DUpper.setOnsetTime(expansionTime + expansionLength + 500);
    DMiddle.setOnsetTime(expansionTime + expansionLength + 500);
    DLower.setOnsetTime(expansionTime + expansionLength + 500);

    subFieldUpper.addEvent (DUpper);
    subFieldMiddle.addEvent(DMiddle);
    subFieldLower.addEvent (DLower);

    temperatureFluctuations tFluctUpper(&thermoUpper);
    temperatureFluctuations tFluctMiddle(&thermoMiddle);
    temperatureFluctuations tFluctLower(&thermoLower);

    subFieldUpper.addEvent (tFluctUpper);
    subFieldMiddle.addEvent(tFluctMiddle);
    subFieldLower.addEvent (tFluctLower);


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

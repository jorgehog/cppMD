#include <iostream>
#include <libconfig_utils/libconfig_utils.h>
#include <ignis.h>
#include <armadillo>

using namespace arma;
using namespace libconfig;
using namespace ignis;


#ifdef USE_MPI
#include <mpi.h>
#endif

#include "cppMD.h"

int main()
{
//    srand(time(NULL));
    wall_clock timer;

    Config cfg;

    std::string configPath = "../infiles/configMD.cfg";
    cfg.readFile(configPath.c_str());

    const Setting & root = cfg.getRoot();

    /*
     * Parsing config file
     */

    const Setting & solver = getSurfaceSetting(root, "solver");

    double dt = getSurfaceSetting<double>(solver, "dt");
    uint N =     getSurfaceSetting<uint>(solver, "N");
    uint therm = getSurfaceSetting<uint>(solver, "therm");
    double m =  getSurfaceSetting<double>(solver, "tightness");
    std::string outputPath = getSurfaceSetting<std::string>(solver, "outputPath");


    double T0 = getSetting<double>(root, {"mainThermostat", "bathTemperature"});
    double tau = getSetting<double>(root, {"mainThermostat", "tauOverDt"})*dt;


    const Setting & ensembleParameters = getSurfaceSetting(root, "ensembleParameters");
    uint nSpecies = getSurfaceSetting<uint>(ensembleParameters, "nSpecies");

    mat sigmaTable(nSpecies, nSpecies);
    mat epsTable(nSpecies, nSpecies);
    vec masses(nSpecies);

    const Setting & sigmas = getSurfaceSetting(ensembleParameters, "sigmas");
    const Setting & epses  = getSurfaceSetting(ensembleParameters, "epses");
    const Setting & _masses = getSurfaceSetting(ensembleParameters, "masses");

    for (uint i = 0; i < nSpecies; ++i) {

        masses(i) = (double)_masses[i];

        for (uint j = 0; j < nSpecies; ++j) {
            sigmaTable(i, j) = 0.5*((double)sigmas[i] +
                                    (double)sigmas[j]);

            epsTable(i, j)   = sqrt((double)epses[i]*
                                    (double)epses[j]);
        }
    }

    assert(sigmaTable(0,0) == 1. && "We are working in reduced units.");
    assert(epsTable(0,0) == 1. && "We are working in reduced units.");
    assert(masses(0) == 1. && "We are working in reduced units.");


#ifndef NO_DCVIZ
    double delay = getSetting<double>(root, {"DCViz", "delay"});
#endif

    const Setting & events = getSurfaceSetting(root, "Events");

    uint compressionTime =        getSetting<uint>(events,    {"Compression", "timeMinusTherm"}) + therm;
    uint compressionLength =      getSetting<uint>(events,    {"Compression", "length"});
    double compressionDelta =    getSetting<double>(events, {"Compression", "delta"});

    uint expansionTime =        getSetting<uint>(events, {"Expansion", "timeMinusCompressionTime"}) + compressionTime;
    uint expansionLength =      getSetting<uint>(events, {"Expansion", "lengthOverCompressionLength"})*compressionLength;
    double expansionDelta = getSetting<double>(events, {"Expansion", "delta"});

    double tScaleWarm = getSetting<double>(events, {"Thermostats", "temperatureScaleFactorWarm"});
    double tScaleCold = getSetting<double>(events, {"Thermostats", "temperatureScaleFactorCold"});

    double tWidth = getSetting<double>(events, {"Thermostats", "widthFactor"});

    double initialDelta = getSetting<double>(root, {"solver", "initialDelta"});

    /*
     * Creating the main mesh
     */

    Ensemble ensemble(masses);

    double Lx = IGNIS_NX*m;
    double Ly = IGNIS_NY*m;

    mat topology(2, 2);
    topology << 0 << Lx << endr << 0 << Ly;

    MainMesh mainMesh(topology, ensemble);
    mainMesh.setOutputPath(outputPath);

    uint therm10 = therm/10;
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
    SaveToFile saveToFile(outputPath, 1);
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
    LauchDCViz launchDCViz(outputPath, delay);
    mainMesh.addEvent(launchDCViz);
#endif
#ifdef USE_QT_GUI
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

    dummy a;
    mainMesh.addEvent(a);


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
#ifndef USE_QT_GUI
    mainMesh.eventLoop(N);
#else
//    stall smoothify(1./60);
//    mainMesh.addEvent(smoothify);
    platform.startAdvanceTimer();
    MainWindow::startMDThread(N, &platform, &mainMesh);
#endif
    std::cout << "Execution time: " << timer.toc() << std::endl;

#ifdef USE_QT_GUI
    return app.exec();
#else
    return 0;
#endif
}

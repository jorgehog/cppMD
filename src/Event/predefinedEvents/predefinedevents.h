#ifndef PREDEFINEDEVENTS_H
#define PREDEFINEDEVENTS_H

#ifndef NO_DCVIZ
#include <DCViz.h>
#endif
#include <assert.h>

#include <boost/lexical_cast.hpp>
#define toStr boost::lexical_cast<std::string>

#include "../../defines.h"
#include "../../Event/event.h"
#include "../../Ensemble/ensemble.h"
#include "../../MeshField/meshfield.h"
#include "../../gears.h"


/*
 *
 * Event for handling periodic boundary conditions
 *
 */

class periodicScaling : public Event {
public:

    periodicScaling() : Event("PeriodicRescale") {}


    //Hey, what I mean to say is that I rescale all positions to fit the mesh _if_ they are set to
    //periodic. If not, you have to add an event to take care of collisions with walls.
    void execute()
    {

        using namespace std;
#if defined (ENS_PERIODIC_X) || defined (ENS_PERIODIC_Y) || defined (ENS_PERIODIC_Z)
        for (int i = 0; i < ENS_N; ++i) {
#ifdef ENS_PERIODIC_X
            if (ensemble->pos(0, i) < meshField->topology(0, 0)) {
                ensemble->pos(0, i) += meshField->shape(0);
            }
            ensemble->pos(0, i) = meshField->topology(0, 0) +
                    fmod(ensemble->pos(0, i) - meshField->topology(0, 0), meshField->shape(0));
#endif
#ifdef ENS_PERIODIC_Y
            if (ensemble->pos(1, i) < meshField->topology(1, 0)) {
                ensemble->pos(1, i) += meshField->shape(1);
            }
            ensemble->pos(1, i) = meshField->topology(1, 0) +
                    fmod(ensemble->pos(1, i) - meshField->topology(1, 0), meshField->shape(1));
#endif
#ifdef ENS_PERIODIC_Z
            if (ensemble->pos(2, i) < meshField->topology(2, 0)) {
                ensemble->pos(2, i) += meshField->shape(2);
            }
            ensemble->pos(2, i) = meshField->topology(2, 0) +
                    fmod(ensemble->pos(2, i), meshField->shape(2));

#endif
        }
#endif
    }

};

/*
 * Velocity verlet. Due to it's nature it has to be split to fit the event system.
 */

class VelocityVerletFirstHalf : public Event {
public:

    VelocityVerletFirstHalf(double dt) : Event("VelVer1"), dt(dt) {}

    void execute() {

        double m;
        for (int i = 0; i < ENS_N; ++i) {

            m = ensemble->masses(i%ensemble->nSpecies);

            for (int k = 0; k < ENS_DIM; ++k) {
                ensemble->vel(k, i) += ensemble->forces(k, i)*dt/(2*m);
                ensemble->pos(k, i) += ensemble->vel(k, i)*dt;
            }

        }

    }

private:
    double dt;

};

class VelocityVerletSecondHalf : public Event {
public:

    VelocityVerletSecondHalf(double dt) : Event("VelVer2"), dt(dt) {}

    void execute() {

        double m;
        for (int i = 0; i < ENS_N; ++i) {

            m = ensemble->masses(i%ensemble->nSpecies);
            for (int k = 0; k < ENS_DIM; ++k) {
                ensemble->vel(k, i) += ensemble->forces(k, i)*dt/(2*m);
            }
        }
    }

private:

    double dt;

};



/*
 *
 *Event for shuffeling positions very randomly..
 *
 */


class randomShuffle : public Event {
public:
    randomShuffle() : Event("shuffling") {}

    void execute() {
        ensemble->pos.randu();
        for (int i = 0; i < ENS_N; ++i) {
            for (int j = 0; j < ENS_DIM; ++j) {
                ensemble->pos(j, i) = meshField->topology(j, 0) + ensemble->pos(j, i)*meshField->shape(j);
            }
        }
    }

};


/*
 *
 * Event for thermostats

 *
 */

class thermostat : public OnsetEvent {
public:
    thermostat(const double & T0, const double & tau, const double & dt) :
        OnsetEvent("Thermostat", "T0", true, true), T0(T0), tau(tau), dt(dt) {}

protected:

    double T0;
    double tau;
    double dt;

    void setT(){
        setValue(gears::getTemperature(meshField));
    }

};

class BerendsenThermostat : public thermostat {
public:
    BerendsenThermostat(const double & T0, const double & tau, const double & dt) :
        thermostat(T0, tau, dt) {}

    void execute() {
        getGamma();

        for (const int & i : meshField->getAtoms()) {
            ensemble->vel(0, i) *= gamma;
            ensemble->vel(1, i) *= gamma;
#if ENS_DIM == 3
            ensemble->vel(2, i) *= gamma;
#endif
        }
    }

protected:

    double gamma;

    void getGamma(){
        setT();
        gamma = sqrt(1 + dt/tau*(T0/getMeasurement() - 1));
    }

};


/*
 *
 * Event for counting atoms
 *
 */


class countAtoms : public Event{
public:

    countAtoms() : Event("Counting atoms", "", true) {}

    void execute(){
        setValue((meshField->getPopulation()/double(ENS_N))/(meshField->getVolume()));
    }

};

class ReportProgress : public Event {
public:

    ReportProgress() : Event("Progress", "%", true) {}

    void execute() {
        setValue(*loopCycle*100.0/N);
    }
};

class ContractMesh : public OnsetEvent {
public:

    //delta = L_new/L_old -- fraction of shrink/expand
    //trigger = at which time should we trigger?
    //xyz = direction (0=x, 1=y ...)
    ContractMesh(double delta, int xyz,
                 int onTime = UNSET_EVENT_TIME,
                 int offTime = UNSET_EVENT_TIME) :
        OnsetEvent("CompressMesh"),
        delta(delta),
        xyz(xyz)
    {
        assert(xyz >= 0);
        assert(xyz < ENS_DIM);
        assert(delta > 0);

        setOnsetTime(onTime);
        setOffsetTime(offTime);
    }

    void initialize() {
        double L0 = meshField->shape(xyz);
        deltaL = (1 - delta)*L0/(eventLength);
    }

    void execute() {

        double L = meshField->shape(xyz); //The length
        double C = meshField->topology(xyz, 0) + L/2; //The centerpoint

        double localDelta = 1 - deltaL/(2*L);


        meshField->stretchField(deltaL, xyz);

        for (int i = 0; i < ENS_N; ++i) {
            ensemble->pos(xyz, i) =  C*(1-localDelta) + localDelta*ensemble->pos(xyz, i);
        }

    }

protected:

    double delta;
    int xyz;
    double deltaL;

};

class ExpandMesh : public ContractMesh {
public:

    ExpandMesh(double delta, int xyz,
               bool pull = false,
               int onTime = UNSET_EVENT_TIME,
               int offTime = UNSET_EVENT_TIME) :
        ContractMesh(delta, xyz, onTime, offTime),
        pull(pull)
    {

        type = "ExpandMesh";
        assert(delta > 1 && "Expansion must have delta>1");

    }

    void execute() {

        meshField->stretchField(deltaL, xyz);

        if (!pull) return;

        double L = meshField->shape(xyz); //The length
        double C = meshField->topology(xyz, 0) + L/2; //The centerpoint
        double localDelta = 1 - deltaL/(2*L);

        for (int i = 0; i < ENS_N; ++i) {
            ensemble->pos(xyz, i) =  C*(1-localDelta) + localDelta*ensemble->pos(xyz, i);
        }

    }


private:

    bool pull;

};


class SaveToFile : public Event {
public:

    SaveToFile(int freq) : Event("SaveData"), freq(freq) {}

    void execute() {
        if ((*loopCycle % freq) == 0) {
            ensemble->pos.save(std::string("/home/jorgehog/tmp/mdPos") + (toStr(*loopCycle) + ".arma"));
        }
    }

private:
    int freq;

};

#ifndef NO_DCVIZ
class LauchDCViz : public Event {

public:

    LauchDCViz(double delay) : Event(), delay(delay), viz("/home/jorgehog/tmp/mdPos0.arma") {}

    void initialize() {
        viz.launch(true, delay, 16, 14);
    }

    void execute() {}

private:

    double delay;

    DCViz viz;

};
#endif

class killMe : public TriggerEvent {
public:

    killMe(int when) : TriggerEvent() {setTrigger(when);}

    void execute() {
        exit(1);
    }

};

class debugSubMeshResize : public Event {
public:

    debugSubMeshResize(MeshField *mainMesh) : Event("debubSubMesh", "", true), mainMesh(mainMesh) {}

    void initialize() {
        R0 = meshField->getVolume()/mainMesh->getVolume();
    }

    void execute(){
        double R = meshField->getVolume()/mainMesh->getVolume();

        if (R != R){
            std::cout << meshField->getVolume() << std::endl;
            std::cout << mainMesh->getVolume() << std::endl;
            exit(1);
        }

        setValue(R/R0);
    }

private:
    MeshField * mainMesh;
    double R0;


};


class stall : public Event {
public:

    stall(double dt) : Event("Stall"), dt(dt) {}

    void execute() {
        usleep(dt*1E6);
    }

private:
    double dt;
};


#endif // PREDEFINEDEVENTS_H

#ifndef PREDEFINEDEVENTS_H
#define PREDEFINEDEVENTS_H

#include <DCViz.h>
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

    periodicScaling() : Event("PS") {}


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

    VelocityVerletFirstHalf(double dt) : Event("VV1"), dt(dt) {}

    void execute() {
        //        std::cout <<"MAX F " << max(ensemble->forces, 1) << std::endl;
        //        std::cout <<"MEAN F " << mean(ensemble->forces, 1) << std::endl;
        //        std::cout <<"STD F " << stddev(ensemble->forces, 1, 1) << std::endl;
        for (int i = 0; i < ENS_N; ++i) {
            for (int k = 0; k < ENS_DIM; ++k) {
                ensemble->vel(k, i) += ensemble->forces(k, i)*dt/2;
                ensemble->pos(k, i) += ensemble->vel(k, i);
            }
        }
    }

private:
    double dt;

};

class VelocityVerletSecondHalf : public Event {
public:

    VelocityVerletSecondHalf(double dt) : Event("VV2"), dt(dt) {}

    void execute() {
        for (int i = 0; i < ENS_N; ++i) {
            for (int k = 0; k < ENS_DIM; ++k) {
                ensemble->vel(k, i) += ensemble->forces(k, i)*dt/2;
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
        OnsetEvent("Thermostat", "T0", true), T0(T0), tau(tau), dt(dt) {}

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

class AddPressure : public TriggerEvent {
public:

    //delta = L_new/L_old -- fraction of shrink/expand
    //trigger = at which time should we trigger?
    //xyz = direction (0=x, 1=y ...)
    AddPressure(double delta, int xyz) :
        TriggerEvent("Pressure"),
        delta(delta),
        xyz(xyz)
    {
        assert(xyz >= 0);
        assert(xyz < ENS_DIM);
        assert(delta > 0);
        assert(delta < 1);
    }

    void execute() {

        double L = meshField->shape(xyz); //The length
        double C = meshField->topology(xyz, 0) + L/2; //The centerpoint

        double deltaL = (1 - delta)*L/2; //The length to remove at each ends

        mat newTopology = meshField->topology;
        newTopology(xyz, 0) += deltaL;
        newTopology(xyz, 1) -= deltaL;

        meshField->setTopology(newTopology);

        double pos;
        double posFromCenter;
        for (int i = 0; i < ENS_N; ++i) {
            pos = ensemble->pos(xyz, i);
            if (pos > L/2){
                posFromCenter = C - pos;
                ensemble->pos(xyz, i) = C - delta*posFromCenter;
            } else {
                posFromCenter = pos - C;
                ensemble->pos(xyz, i) = C + delta*posFromCenter;
            }

        }
    }

private:

    double delta;
    int xyz;

};


class SaveToFile : public OnsetEvent {
public:

    SaveToFile(int freq) : OnsetEvent(), freq(freq) {}

    void execute() {
        if ((*loopCycle % freq) == 0) {
            ensemble->pos.save(std::string("/home/jorgehog/tmp/mdPos") + (toStr(*loopCycle) + ".arma"));
        }
    }

private:
    int freq;

};

class LauchDCViz : public TriggerEvent {

public:

    LauchDCViz(int delay) : TriggerEvent(), delay(delay), viz("/home/jorgehog/tmp/mdPos0.arma") {}

    void execute() {
        viz.launch(true, delay);
    }

private:

    int delay;

    DCViz viz;

};


#endif // PREDEFINEDEVENTS_H

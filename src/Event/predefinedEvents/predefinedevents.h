#ifndef PREDEFINEDEVENTS_H
#define PREDEFINEDEVENTS_H

#include "../../defines.h"
#include "../../Event/event.h"
#include "../../Ensemble/ensemble.h"
#include "../../MeshField/meshfield.h"
#include "../../MeshField/MainMesh/mainmesh.h"
#include "../../MD/mdsolver.h"


/*
 *
 * Event for handling periodic boundary conditions
 *
 */

class periodicScaling : public Event {
public:

    periodicScaling() : Event("PS", "", false) {}


    //Hey, what I mean to say is that I rescale all positions to fit the mesh _if_ they are set to
    //periodic. If not, you have to add an event to take care of collisions with walls.
    void execute()
    {

#if defined (ENS_PERIODIC_X) || defined (ENS_PERIODIC_Y) || defined (ENS_PERIODIC_Z)
        for (int i = 0; i < ENS_N; ++i) {
#ifdef ENS_PERIODIC_X
            ensemble->pos(0, i) = fmod(ensemble->pos(0, i), meshField->topology(0, 1));
#endif
#ifdef ENS_PERIODIC_Y
            ensemble->pos(1, i) = fmod(ensemble->pos(1, i), meshField->topology(1, 1));
#endif
#ifdef ENS_PERIODIC_Z
            ensemble->pos(2, i) = fmod(ensemble->pos(2, i), meshField->topology(2, 1));
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

    VelocityVerletFirstHalf(double dt) : Event("VV1", "", false), dt(dt) {}

    void execute() {
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

    VelocityVerletSecondHalf(double dt) : Event("VV2", "", false), dt(dt) {}

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
    randomShuffle() : Event("shuffling", "", false) {}

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
 * Event for counting atoms
 *
 */


class countAtoms : public Event{
public:

    countAtoms() : Event("Counting atoms") {}

    void execute(){
        setValue((meshField->getPopulation()/double(ENS_N))/(meshField->getVolume()));
    }

};


#endif // PREDEFINEDEVENTS_H

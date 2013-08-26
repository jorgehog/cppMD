#ifndef PREDEFINEDEVENTS_H
#define PREDEFINEDEVENTS_H

#include "../../defines.h"
#include "../../Event/event.h"
#include "../../Ensemble/ensemble.h"
#include "../../MeshField/meshfield.h"

/*
 *
 * Event for handling periodic boundary conditions
 *
 */

class periodicScaling : public Event {
public:
    //Hey, what I mean to say is that I rescale all positions to fit the mesh _if_ they are set to
    //periodic. If not, you have to add an event to take care of collisions with walls.
    void execute()
    {
#ifdef ENS_PERIODIC_X || ENS_PERIODIC_Y || ENS_PERIODIC_Z
        for (int i = 0; i < ENS_N; ++i) {
#ifdef ENS_PERIODIC_X
            ensemble->pos(i, 0) = fmod(ensemble->pos(i,0), meshField->topology(0, 1));
#endif
#ifdef ENS_PERIODIC_Y
            ensemble->pos(i, 1) = fmod(ensemble->pos(i,1), meshField->topology(1, 1));
#endif
#ifdef ENS_PERIODIC_Z
            ensemble->pos(i, 2) = fmod(ensemble->pos(i,2), meshField->topology(2, 1));
#endif
        }
#endif
    }

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
                ensemble->pos(i,j) = meshField->topology(j, 1) + ensemble->pos(i,j)*meshField->shape(j);
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

    void execute(){
        setValue((meshField->getPopulation()/double(ENS_N))/(meshField->getVolume()));
    }

};

#endif // PREDEFINEDEVENTS_H

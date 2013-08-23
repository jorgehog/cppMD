#ifndef SOLVEREVENT_H
#define SOLVEREVENT_H

#include "../event.h"

class SolverEvent : public Event
{
public:

    SolverEvent();

    bool isSolverEvent(){
        return true;
    }

    void apply(int i){

    }


};

#endif // SOLVEREVENT_H

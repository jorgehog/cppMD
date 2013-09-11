#ifndef FORCES_H
#define FORCES_H

#include "../../Event/event.h"

#include "../../defines.h"
#include <armadillo>

using namespace arma;

class mdSolver;
class SolverEvent;

class Force : public Event {
public:
    Force();

    virtual void initialize() {
        execute();
    }

protected:


    vec::fixed<ENS_DIM> force;
    ivec::fixed<3> ldi; //least distance iterator

    void leastDistance(vec & leastRel, double &leastRel2, const int & i, const int & j);

};

class TwoBodyForce : public Force {
public:

    TwoBodyForce() : Force() {}

    virtual const vec getForce(const int&  i, const int& j) = 0;

    void execute();

protected:

    double rRel2;
    vec::fixed<ENS_DIM> rRel;

};


/*
 * Calculating lennard jones force
 */

class LennardJonesForce : public TwoBodyForce {
public:

    LennardJonesForce(mat & sigmaTable, mat & epsTable);

    const vec getForce(const int &i, const int &j);

private:

    const int nSpecies;

    mat sigmaTable;
    mat epsTable;

};

#endif //FORCES_H

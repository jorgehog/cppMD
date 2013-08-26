#ifndef CALCULATEFORCES_H
#define CALCULATEFORCES_H

#include "../../../Event/event.h"

#include "../../../defines.h"
#include <armadillo>

using namespace arma;

class mdSolver;
class SolverEvent;

/*
 * Calculating lennard jones force
 */
class TwoBodyForce : public Event {
public:

    TwoBodyForce(mdSolver * solver, int nSpecies);

    void initialize() {
        execute();
    }

    virtual const vec getForce(const int&  i, const int& j) = 0;

    void execute();

protected:

    int nSpecies;

    double rRel2;

    SolverEvent* solver;

    vec::fixed<ENS_DIM> rRel;
    vec::fixed<ENS_DIM> force;

};

class LennardJonesForce : public TwoBodyForce {
public:

    LennardJonesForce(mdSolver* solver, int nSpecies);

    const vec getForce(const int &i, const int &j);

private:

    mdSolver * mdsolver;

};

#endif //CALCULATEFORCES_H

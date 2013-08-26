#ifndef MDSOLVER_H
#define MDSOLVER_H

#include "../Event/SolverEvent/solverevent.h"

class mdSolver : public SolverEvent
{
public:

    mdSolver(double dt, int N, int nSpecies, const double *sigmas, const double *epses);


    friend class LennardJonesForce;

private:

    int nSpecies;

    mat sigmaTable;
    mat epsTable;

    void initialize();



};

#endif // MDSOLVER_H

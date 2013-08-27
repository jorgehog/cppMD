#ifndef MDSOLVER_H
#define MDSOLVER_H

#include "../Event/SolverEvent/solverevent.h"

class mdSolver : public SolverEvent
{
public:

    mdSolver(double dt, int N, int nSpecies, double T0, const double *sigmas, const double *epses);


    friend class LennardJonesForce;

private:

    int nSpecies;
    double T0;
    double sqrtkT0;

    mat sigmaTable;
    mat epsTable;

    void initialize();



};

#endif // MDSOLVER_H

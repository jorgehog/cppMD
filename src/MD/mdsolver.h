#ifndef MDSOLVER_H
#define MDSOLVER_H

#include "../Event/SolverEvent/solverevent.h"

class mdSolver : public SolverEvent
{
public:

    mdSolver(double dt, int N, double T0);

private:

    double sqrtkT0;

    void initialize();

};

#endif // MDSOLVER_H

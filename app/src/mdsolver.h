#ifndef MDSOLVER_H
#define MDSOLVER_H

#include <ignis.h>
#include <math.h>

using namespace ignis;

class mdSolver : public Event
{
public:

    mdSolver(const uvec & N, double T0, double dt);

private:

    const uvec N;

    double sqrtkT0;
    double dt;

    void initialize();
    void execute() {}

};

#endif // MDSOLVER_H

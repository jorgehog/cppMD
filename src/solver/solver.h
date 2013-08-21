#ifndef SOLVER_H
#define SOLVER_H

class Ensemble;
class Mesh;


class solver
{
public:

    solver(double dt, Mesh *mesh, Ensemble *atoms);


    void run(int nSteps);

private:

    double dt;

    Mesh *mesh;
    Ensemble *atoms;


    void initialize();

};

#endif // SOLVER_H

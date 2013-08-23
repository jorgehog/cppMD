#ifndef MD_SOLVER_H
#define MD_SOLVER_H

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

#endif // MD_SOLVER_H

#include "solver.h"
#include <iostream>


solver::solver(double dt, Mesh *mesh, Ensemble *atoms)
{
    this->mesh = mesh;
    this->atoms = atoms;

    this->dt = dt;

    std::cout << "heiz" << std::endl;

}

void solver::run(int nSteps)
{

    initialize();


    int cycle = 0;

    while (cycle != nSteps) {

//        Iterate stuff


    }

}

void solver::initialize()
{

//    Set initial stuff and stuff


}

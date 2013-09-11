#include "mainmesh.h"

#include <assert.h>

#include <boost/lexical_cast.hpp>
#define toStr boost::lexical_cast<std::string>

#include "../../Event/SolverEvent/solverevent.h"
#include "../../Ensemble/ensemble.h"


MainMesh::MainMesh(const mat &topology, Ensemble  & ensemble):
    MeshField(topology, ensemble, "MainMesh"),
    solver(NULL)
{

    for (int i = 0; i < ENS_N; ++i) {
        atoms.push_back(i);
    }

}


void MainMesh::updateContainments()
{

    for (MeshField* subField : subFields){
        subField->resetSubFields();
    }

    for (int i = 0; i < ENS_N; ++i) {

        for (MeshField* subField : subFields){
            (void)subField->checkSubFields(i);
        }

    }

}

void MainMesh::addSolverEvent(SolverEvent &solver)
{
    solver.setMeshField(this);
    solver.setEnsemble(ensemble);
    this->solver = &solver;
}

void MainMesh::eventLoop()
{
    assert(solver != NULL);

    solver->initialize();
    initializeEvents();

    double T = 0;
    const double &dt = solver->dt;
    const int &N = solver->N;

    for (int i = 0; i < N; ++i, T+=dt) {

        //TMP
        ensemble->pos.save(std::string("/home/jorgehog/tmp/mdPos") + (toStr(i) + ".arma"));
        //

        updateContainments();       //1. Find which atoms are in which meshes

        executeEvents();            //2. Let each mesh execute their events on these atoms
        dumpEvents();               //3. Let each event dump their output

        resetEvents();              //4. Reset.

        std::cout << "t = " << T << " / " << (N-1)*dt << std::endl;

    }
}



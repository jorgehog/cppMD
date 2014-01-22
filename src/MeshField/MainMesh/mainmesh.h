#ifndef MAINMESH_H
#define MAINMESH_H

#include "../meshfield.h"

class MainMesh : public MeshField
{

public:
    MainMesh(const mat & topology, Ensemble &ensemble);

    int getPopulation() const {
        return ENS_N;
    }

    void eventLoop(int N);

    const int *getLoopCounter(){
        return loopCounter;
    }

    void setOutputPath(std::string path);

private:

    void updateContainments();
    void dumpEventsToFile();

    std::string outputPath;


};

#endif // MAINMESH_H

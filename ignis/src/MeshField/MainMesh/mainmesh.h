#ifndef MAINMESH_H
#define MAINMESH_H

#include "../meshfield.h"

class MainMesh : public MeshField
{

public:
    MainMesh(const mat & topology, Ensemble &ensemble);

    uint getPopulation() const {
        return ENS_N;
    }

    void eventLoop(uint N);

    void setOutputPath(std::string path);

private:

    void sendToTop(Event &event);

    void sortEvents();

    void updateContainments();

    void dumpEventsToFile();

    std::string outputPath;

    std::vector<Event*> allEvents;



};

#endif // MAINMESH_H

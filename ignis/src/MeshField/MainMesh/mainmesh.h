#ifndef MAINMESH_H
#define MAINMESH_H

#include "../meshfield.h"

namespace ignis
{

class MainMesh : public MeshField
{

public:
    MainMesh(const mat & topology, Ensemble &ensemble);

    uint getPopulation() const {
        return IGNIS_N;
    }

    void eventLoop(uint N);

    void setOutputPath(std::string path);

    void dumpEvents() const;

    void dumpEventsToFile() const;


private:

    void sendToTop(Event &event);


    void addIntrinsicEvents();


    void sortEvents();

    void executeEvents();



    void updateContainments();


    std::string outputPath;

    std::vector<Event*> allEvents;



};
}

#endif // MAINMESH_H

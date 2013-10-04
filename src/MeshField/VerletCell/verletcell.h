#ifndef VERLETCELL_H
#define VERLETCELL_H

#include "../meshfield.h"
#include "../MainMesh/mainmesh.h"

class VerletCell : public MeshField
{
public:
//    VerletCell(MainMesh* mainMesh, int i, int j, int k);

private:

    MainMesh* mainMesh;
    uvec cellNumbers;


    bool isWithinThis(int i){
        (void) i;
        return false;

    }


};

#endif // VERLETCELL_H

#include "region.h"

#include "../Ensemble/ensemble.h"

Region::Region(const mat &topology, Ensemble *ensemble):
    topology(topology),
    ensemble(ensemble)
{

}

void Region::append(int i)
{

    //Check whether atom 'i' is outside any of the region's bounds
    for (int j = 0; j < DIM; ++j) {
        if (ensemble->pos(i, j) < topology(0, j)){
            return;
        } else if (ensemble->pos(i, j) > topology(1, j)) {
            return;
        }
    }

    atoms.push_back(i);

}

bool Region::equals(const Region &region) const
{

       for (int i = 0; i < 2; ++i) {
           for (int j = 0; j < DIM; ++j) {
               if (region.topology(i, j) != this->topology(i, j)) {
                   return false;
               }
           }
       }

       return true;

   }

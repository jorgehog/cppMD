#pragma once

#include <ignis.h>

#include <armadillo>

using namespace arma;

class MDParticles : public ignis::PositionHandler<double>
{
public:

    MDParticles(const vec & masses) :
        masses(masses)
    {
        forceVectors.set_size(MD_N, MD_N, IGNIS_DIM);
    }

    double operator() (const uint n, const uint d) const
    {
        return pos(n, d);
    }

    double &operator() (const uint n, const uint d)
    {
        return pos(n, d);
    }


    uint count() const
    {
        return MD_N;
    }

    const uint & nSpecies() const
    {
        return masses.n_elem;
    }

    uint NX() const
    {
        return MD_NX;
    }

    uint NY() const
    {
        return MD_NY;
    }

#if IGNIS_DIM == 3
    uint NZ() const
    {
        return MD_NZ;
    }
#endif


    mat::fixed<IGNIS_DIM, MD_N> pos;

    mat::fixed<IGNIS_DIM, MD_N> vel;

    mat::fixed<IGNIS_DIM, MD_N> forces;

    cube forceVectors;

    const vec masses;

};

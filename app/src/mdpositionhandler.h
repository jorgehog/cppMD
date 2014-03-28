#pragma once

#include <ignis.h>

#include <armadillo>

using namespace arma;

#if IGNIS_DIM == 2
#define _MDParticles_Decl_ template<const uint _NX, const uint _NY, const uint _N = _NX*_NY>
#define _MDParticles_Templ_ <_NX, _NY>
#else
#define _MDParticles_Decl_ template<const uint _NX, const uint _NY, const uint _NZ, const uint _N = _NX*_NY*_NZ>
#define _MDParticles_Templ_ <_NX, _NY, _NZ>
#endif

#define _MDParticles_Type_ MDParticles _MDParticles_Templ_

_MDParticles_Decl_
class MDParticles : ignis::PositionHandler<double>
{
public:

    MDParticles(const vec & masses) :
        masses(masses)
    {

    }

    virtual double &at(const uint n, const uint d) const
    {
        return *(pos.memptr()+ n*IGNIS_DIM + d);
    }


    uint count() const
    {
        return _N;
    }

    const uint & nSpecies() const
    {
        return masses.n_elem;
    }

    uint NX() const
    {
        return _NX;
    }

    uint NY() const
    {
        return _NY;
    }

#if IGNIS_DIM == 3
    uint NZ() const
    {
        return _NZ;
    }
#endif


    mat::fixed<IGNIS_DIM, _N> pos;

    mat::fixed<IGNIS_DIM, _N> vel;

    mat::fixed<IGNIS_DIM, _N> forces;

    cube::fixed<_N, _N, IGNIS_DIM> forceVectors;

    const vec masses;

};

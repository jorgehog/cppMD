#pragma once

#include <ignis.h>

#include "gears.h"

using namespace ignis;

/*
 * Velocity verlet. Due to it's nature it has to be split to fit the event system.
 */

class VelocityVerletFirstHalf : public Event {
public:

    VelocityVerletFirstHalf(double dt) : Event("VelVer1"), dt(dt) {}

    void execute() {

        double m;

        for (uint i = 0; i < positions->count(); ++i) {

            m = particles->masses(i%particles->nSpecies());

            for (uint k = 0; k < IGNIS_DIM; ++k) {
                particles->vel(k, i) += particles->forces(k, i)*dt/(2*m);
                particles->pos(k, i) += particles->vel(k, i)*dt;
            }

        }

    }

private:
    double dt;

};

class VelocityVerletSecondHalf : public Event {
public:

    VelocityVerletSecondHalf(double dt) : Event("VelVer2"), dt(dt) {}

    void execute() {

        double m;

        for (uint i = 0; i < positions->count(); ++i) {

            m = particles->masses(i%particles->nSpecies());
            for (uint k = 0; k < IGNIS_DIM; ++k) {
                particles->vel(k, i) += particles->forces(k, i)*dt/(2*m);
            }
        }
    }

private:

    double dt;

};



/*
 *
 * Event for thermostats
 *
 */

class thermostat : public Event {
public:
    thermostat(const double & T0, const double & tau, const double & dt) :
        Event("Thermostat", "T0", true, true), T0(T0), tau(tau), dt(dt) {}

protected:

    double T0;
    double tau;
    double dt;

    void setT(){
        setValue(gears::getTemperature(meshField));
    }



};

class BerendsenThermostat : public thermostat {
public:
    BerendsenThermostat(const double & T0, const double & tau, const double & dt,
                        uint onTime = IGNIS_UNSET_UINT,
                        uint offTime = IGNIS_UNSET_UINT) :
        thermostat(T0, tau, dt) {
        setOnsetTime(onTime);
        setOffsetTime(offTime);
    }

    void execute() {
        getGamma();

        for (const uint & i : meshField->getAtoms()) {
            particles->vel(0, i) *= gamma;
            particles->vel(1, i) *= gamma;
#if IGNIS_DIM == 3
            particles->vel(2, i) *= gamma;
#endif
        }
    }

protected:

    double gamma;

    void getGamma(){
        setT();
        gamma = sqrt(1 + dt/tau*(T0/getMeasurement() - 1));
    }

};



class pressureMOP : public Event {
public:

    pressureMOP(uint xyz) : Event("Pressure", "P0", true, true), xyz(xyz) {}

    void initialize() {
        mat topology1 = meshField->topology;
        mat topology2 = meshField->topology;

        topology1(xyz, 1) = topology1(xyz, 0) + meshField->shape(xyz)/2;
        topology2(xyz, 0) = topology1(xyz, 1);

        box1 = new MeshField(topology1, *particles, "Method of Planes 1. field");
        box2 = new MeshField(topology2, *particles, "Method of Planes 2. field");

        meshField->addSubField(*box1);
        meshField->addSubField(*box2);

        area = 1;

        for (uint i = 0; i < IGNIS_DIM; ++i) {
            if (i != xyz) {
                area *= meshField->shape(i);
            }
        }

    }

    double getPressureMOP() {

        double planeForce = 0;

        for (const uint & i : box1->getAtoms()) {
            for (const uint & j : box2->getAtoms()) {
                planeForce += particles->forceVectors(i, j, xyz);
            }
        }


        return fabs(planeForce/area);
    }

    void execute() {

        double pressure = getPressureMOP();

        setValue(pressure);
    }

private:
    uint xyz;
    double area;

    MeshField* box1;
    MeshField* box2;


};


class checkEnergyConservation : public Event {
public:

    checkEnergyConservation() : Event("energyConservation", "", true) {}

    void initialize() {
        EkPrev = gears::getKineticEnergy(meshField);
    }

    void execute() {
        Ek = gears::getKineticEnergy(meshField);

        setValue((Ek-EkPrev)/EkPrev);
    }

    void reset() {
        EkPrev = Ek;
    }

private:
    double Ek;
    double EkPrev;
};

class checkMomentumConservation : public Event {
public:

    checkMomentumConservation() : Event("momentumConservation", "", true) {}

    void execute() {
        vec p = gears::getTotalLinearMomentum(particles);

        setValue(as_scalar(sum(p)));
    }

};


class diffusionConstant : public Event {
public:

    diffusionConstant(double dt) : Event("DiffusionConstant", "D0", true), fac(dt/(IGNIS_DIM)), D(0) {}

    void initialize() {
        v0 = particles->vel;
    }

    void execute() {

        double dD = 0;

        for (const uint & i : meshField->getAtoms()) {
            for (uint j = 0; j < IGNIS_DIM; ++j) {
                dD += v0(j, i)*particles->vel(j, i);
            }
        }

        dD /= meshField->getPopulation();

        if (dD != dD) {
            return;
        }

        D += dD*fac;

        setValue(D);

    }

private:

    mat v0;
    double fac;
    double D;

};

class temperatureFluctuations : public Event {
public:

    temperatureFluctuations(thermostat *t) : Event("TempFluct", "T0", true), t(t) {
        setOnsetTime(t->getOnsetTime());
        setOffsetTime(t->getOffsetTime());
    }

    void execute() {

        double T = t->getMeasurement();

        if (T != T) {
            setValue(0);
            return;
        }

        avgT += T;
        avgT2 += T*T;

        setValue(sqrt(avgT2/(nTimesExecuted+1) - avgT*avgT/((nTimesExecuted+1)*(nTimesExecuted+1))));

    }

private:

    thermostat* t;

    double avgT;
    double avgT2;

};

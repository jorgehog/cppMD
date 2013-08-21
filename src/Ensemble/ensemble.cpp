
#include "ensemble.h"


Ensemble::Ensemble(Thermostat *thermostat)
{

    this->thermostat = thermostat;

    std::cout << pos + vel + forces << std::endl;

}

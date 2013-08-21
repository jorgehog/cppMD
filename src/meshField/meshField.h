#ifndef FIELD_H
#define FIELD_H

#include <ostream>

class Region;

class meshField
{

private:

    std::string type;
    std::string unit;

    const Region *reg;

public:

    virtual void update() = 0;

    virtual double getMeasurement(){
        return 0.0;
    }

    void addRegion(const Region & region){
        reg = &region;
    }

    std::string dumpString();


};

#endif // FIELD_H

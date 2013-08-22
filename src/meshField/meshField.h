#ifndef MD_MESHFIELD_H
#define MD_MESHFIELD_H

#include <string>

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

#endif // MD_MESHFIELD_H

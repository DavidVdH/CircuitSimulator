#ifndef RESISTOR_H
#define RESISTOR_H

#include "component.h"

using namespace std;

class Resistor : public Component
{
public:
    int resistance;

    Resistor();
    Resistor(string, bool, int);

};

#endif // RESISTOR_H

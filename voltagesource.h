#ifndef VOLTAGESOURCE_H
#define VOLTAGESOURCE_H

#include "component.h"

class VoltageSource : public Component
{
public:
    int voltage;

    VoltageSource();
    VoltageSource(string, bool, int);

    bool isPositiveGate(Wire*);
};

#endif // VOLTAGESOURCE_H

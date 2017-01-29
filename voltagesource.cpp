#include "voltagesource.h"

VoltageSource::VoltageSource()
{
}

VoltageSource::VoltageSource(string name, bool isHorizontal, int voltage):
    Component(name)
{
    maximumNumberConnections = 2;

    this->voltage = voltage;

    QPixmap tempHorPixmap("/home/david/Documents/Programmeren/Project/voltageHor.png");
    horizontalPixmap = tempHorPixmap.scaled(2*gridSize,2*gridSize);

    QPixmap tempVertPixmap("/home/david/Documents/Programmeren/Project/voltageVert.png");
    verticalPixmap = tempVertPixmap.scaled(2*gridSize,2*gridSize);

    QPixmap tempHorSelPixmap("/home/david/Documents/Programmeren/Project/voltageHorSel.png");
    horizontalSelPixmap = tempHorSelPixmap.scaled(2*gridSize,2*gridSize);

    QPixmap tempVertSelPixmap("/home/david/Documents/Programmeren/Project/voltageVertSel.png");
    verticalSelPixmap = tempVertSelPixmap.scaled(2*gridSize,2*gridSize);

    this->setOrientation(isHorizontal);

    initialiseConnectionPoints(maximumNumberConnections);
}


// Checks if a wire is connected to the positive or negative port
bool VoltageSource::isPositiveGate(Wire *wire)
{
    if(connectionPoints.at(1).second == wire){
        return true;
    }else{
        return false;
    }
}

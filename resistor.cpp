#include "resistor.h"

#include <QPixmap>

Resistor::Resistor()
{
}

Resistor::Resistor(string name, bool isHorizontal, int resistance):
    Component(name)
{
    maximumNumberConnections = 2;

    this->resistance = resistance;

    QPixmap tempHorPixmap("/home/david/Documents/Programmeren/Project/resistorHor.png");
    horizontalPixmap = tempHorPixmap.scaled(2*gridSize,2*gridSize);

    QPixmap tempVertPixmap("/home/david/Documents/Programmeren/Project/resistorVert.png");
    verticalPixmap = tempVertPixmap.scaled(2*gridSize,2*gridSize);

    QPixmap tempHorSelPixmap("/home/david/Documents/Programmeren/Project/resistorHorSel.png");
    horizontalSelPixmap = tempHorSelPixmap.scaled(2*gridSize,2*gridSize);

    QPixmap tempVertSelPixmap("/home/david/Documents/Programmeren/Project/resistorVertSel.png");
    verticalSelPixmap = tempVertSelPixmap.scaled(2*gridSize,2*gridSize);


    this->setOrientation(isHorizontal);

    initialiseConnectionPoints(maximumNumberConnections);

}

#include "component.h"
#include "wire.h"

#include <QMessageBox>
#include <QPair>
#include <vector>
#include <QDebug>

Component::Component(string name, QGraphicsItem *parent) :
    QGraphicsPixmapItem(parent)
{
    gridSize = 25;
    this->name = name;
    nameLabel = new QGraphicsSimpleTextItem(QString::fromStdString(name),this);
    nameLabel->setFont(QFont("Helvetica [Cronyx]", 8));
}


Component::Component()
{
}


Component::~Component()
{
    // remove all wires attached to the component
    while(wires.size() > 0){
       delete wires.at(0);
    }
}


// Changes component pixMap when it is selected.
void Component::setSelection(bool isSelected)
{

    if(isHorizontal){
        if(isSelected){
            this->setPixmap(horizontalSelPixmap);
        }else{
            this->setPixmap(horizontalPixmap);
        }
    }else{
        if(isSelected){
            this->setPixmap(verticalSelPixmap);
        }else{
            this->setPixmap(verticalPixmap);
        }
    }
}

// Changes the component pixMap based on the orientation
void Component::setOrientation(bool isHorizontal)
{
    this->isHorizontal = isHorizontal;

    if(isHorizontal){
        this->setPixmap(horizontalPixmap);
    }else{
        this->setPixmap(verticalPixmap);
    }
}

// Fills the vector 'connectionPoints' with QPairs with NULL values
void Component::initialiseConnectionPoints(int maximumNumberConnections)
{
    for(int i = 0; i < maximumNumberConnections; i++){
        QPair<QPointF, Wire*> connectionPoint(QPointF(0, 0), NULL);
        connectionPoints.push_back(connectionPoint);
    }
}


// Updates the coordinates of the connectionPoints. This function is called
// when the component is moved or when the orientation has changed.
void Component::updateConnectionPoints()
{
    // First connectionPoint (index 0) corresponds to left or bottom connection
    // (depending on orientation). The second connection point (index 1) to top
    // or right connectionPoint.
    if(isHorizontal){
        connectionPoints.at(0).first = this->scenePos() + QPointF(0, gridSize);
        connectionPoints.at(1).first = this->scenePos() + QPointF(2*gridSize, gridSize);
    }else{
        connectionPoints.at(0).first = this->scenePos() + QPointF(gridSize, 2*gridSize);
        connectionPoints.at(1).first = this->scenePos() + QPointF(gridSize, 0);
    }
}


// Add wire to list of connected wires
void Component::addWire(Wire *connectedWire)
{
    wires.push_back(connectedWire);
}


// Remove wire from list of connected wires
void Component::removeWire(Wire *wire)
{
    vector<Wire*>::iterator wiresIt;

    wiresIt = find(wires.begin(), wires.end(), wire);
    wires.erase(wiresIt);

    // Reset connectionPoint to which the wire was connected
    for(unsigned int i = 0; i < connectionPoints.size(); i++){
        if(connectionPoints.at(i).second == wire){
            connectionPoints.at(i).second = NULL;
        }
    }
}


// Check if this component is already connected to another component
// via one of the wires. This function is used to prevent that two
// components get connected more than once.
bool Component::checkConnected(Component *component){
    bool connected = false;

    for(unsigned int i = 0; i < wires.size();i++){
        if (wires.at(i)->componentOne == component || wires.at(i)->componentTwo == component){
            connected = true;
        }
    }
    return connected;
}


// This function returns the wire that is not equal to the wire given to it.
// This function is called when the voltages are calculate to make a step
// in the circuit from one component to another.
Wire* Component::getConnectedWire(Wire *wire)
{
    for(unsigned int i = 0; i < wires.size(); i++){
        if(wires.at(i) != wire){
            return wires.at(i);
        }
    }
}

#include "wire.h"
#include "circuitcanvas.h"

#include <QPen>
#include <QDebug>

Wire::Wire(Component *componentOne, Component *componentTwo,CircuitCanvas *scene)
{
    this->componentOne = componentOne;
    this->componentTwo = componentTwo;
    this->scene = scene;

    // Defining the color and width of the wires

    wireOne = new QGraphicsLineItem();
    wireOne->setPen(QPen(Qt::black,4));
    wireTwo = new QGraphicsLineItem();
    wireTwo->setPen(QPen(Qt::black,4));

    // Adding the wires to the scene

    scene->addItem(wireOne);
    scene->addItem(wireTwo);

    // Make sure the wires are on the background at all times

    wireOne->setZValue(-1);
    wireTwo->setZValue(-1);

    // Check how two components are positioned with respect to one another

    LeftToRightOneTwo = (componentOne->scenePos().x() < componentTwo->scenePos().x());
    BottomTopOneTwo = (componentOne->scenePos().y() > componentTwo->scenePos().y());

    // Setup geometry of the wires

    findWireConnections();
    updateWire();
}


Wire::~Wire()
{
    componentOne->removeWire(this);
    componentTwo->removeWire(this);
    scene->removeItem(wireOne);
    scene->removeItem(wireTwo);
    delete wireOne;
    delete wireTwo;
}


// Find which connection points are best used to connect the two components
void Wire::findWireConnections()
{
    if(componentOne->isHorizontal){
        if(LeftToRightOneTwo && componentOne->connectionPoints.at(1).second == NULL){
            indexCompOne = 1;
        }else if(LeftToRightOneTwo && componentOne->connectionPoints.at(1).second != NULL){
            indexCompOne = 0;
        }else if((!LeftToRightOneTwo) && componentOne->connectionPoints.at(0).second == NULL){
            indexCompOne = 0;
        }else if((!LeftToRightOneTwo) && componentOne->connectionPoints.at(0).second != NULL){
            indexCompOne = 1;
        }
    }else if(!componentOne->isHorizontal){
        if(BottomTopOneTwo && componentOne->connectionPoints.at(1).second == NULL){
            indexCompOne = 1;
        }else if(BottomTopOneTwo && componentOne->connectionPoints.at(1).second != NULL){
            indexCompOne = 0;
        }else if((!BottomTopOneTwo) && componentOne->connectionPoints.at(0).second == NULL){
            indexCompOne = 0;
        }else if((!BottomTopOneTwo) && componentOne->connectionPoints.at(0).second != NULL){
            indexCompOne = 1;
        }
    }

    if(componentTwo->isHorizontal){
        if(LeftToRightOneTwo && componentTwo->connectionPoints.at(0).second == NULL){
            indexCompTwo = 0;
        }else if(LeftToRightOneTwo && componentTwo->connectionPoints.at(0).second != NULL){
            indexCompTwo = 1;
        }else if((!LeftToRightOneTwo) && componentTwo->connectionPoints.at(1).second == NULL){
            indexCompTwo = 1;
        }else if((!LeftToRightOneTwo) && componentTwo->connectionPoints.at(1).second != NULL){
            indexCompTwo = 0;
        }
    }else if(!componentTwo->isHorizontal){
        if(BottomTopOneTwo && componentTwo->connectionPoints.at(0).second == NULL){
            indexCompTwo = 0;
        }else if(BottomTopOneTwo && componentTwo->connectionPoints.at(0).second != NULL){
            indexCompTwo = 1;
        }else if((!BottomTopOneTwo) && componentTwo->connectionPoints.at(1).second == NULL){
            indexCompTwo = 1;
        }else if((!BottomTopOneTwo) && componentTwo->connectionPoints.at(1).second != NULL){
            indexCompTwo = 0;
        }
    }

    componentOne->connectionPoints.at(indexCompOne).second = this; // attach wire to connectionPoint of component
    componentTwo->connectionPoints.at(indexCompTwo).second = this;

}

// This function is called when an object changes position or orientation
void Wire::updateWire()
{
    QPointF firstPoint =  componentOne->connectionPoints.at(indexCompOne).first;
    QPointF secondPoint =  componentTwo->connectionPoints.at(indexCompTwo).first;

    if(componentOne->isHorizontal == componentTwo->isHorizontal){
        wireOne->setLine(firstPoint.x(),firstPoint.y(),secondPoint.x(),firstPoint.y());
        wireTwo->setLine(secondPoint.x(),firstPoint.y(),secondPoint.x(),secondPoint.y());
    }else if(componentOne->isHorizontal && !componentTwo->isHorizontal){
        wireOne->setLine(firstPoint.x(),firstPoint.y(),secondPoint.x(),firstPoint.y());
        wireTwo->setLine(secondPoint.x(),firstPoint.y(),secondPoint.x(),secondPoint.y());
    }else if(!componentOne->isHorizontal && componentTwo->isHorizontal){
        wireOne->setLine(firstPoint.x(),firstPoint.y(),firstPoint.x(),secondPoint.y());
        wireTwo->setLine(firstPoint.x(),secondPoint.y(),secondPoint.x(),secondPoint.y());
    }
}

// Returns the component that is attached to the wire and that is not equal to the given component
Component* Wire::getConnectedComponent(Component *component)
{
    if(componentOne == component){
        return componentTwo;
    }else{
        return componentOne;
    }
}




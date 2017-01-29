#include "node.h"

#include <QDebug>

using namespace std;

int Node::numberOfNodes = 0; // static variable


Node::Node(string name) :
    Component(name)
{
    maximumNumberConnections = 3;

    nodeId = numberOfNodes++;

    QPixmap tempHorPixmap("/home/david/Documents/Programmeren/Project/node.png");
    horizontalPixmap = tempHorPixmap.scaled(2*gridSize,2*gridSize);

    QPixmap tempHorSelPixmap("/home/david/Documents/Programmeren/Project/nodeSel.png");
    horizontalSelPixmap = tempHorSelPixmap.scaled(2*gridSize,2*gridSize);

    // No vertical pixmap

    this->setOrientation(true);

    initialiseConnectionPoints(maximumNumberConnections);
}


Node::Node()
{
}


Node::~Node()
{
    Node::numberOfNodes--;
}


// overloaded because the implementation in Component.cpp is based on a component with two connection points
void Node::updateConnectionPoints()
{
    for(unsigned int i = 0; i < connectionPoints.size(); i++){
        connectionPoints.at(i).first = this->scenePos() + QPointF(gridSize, gridSize);
    }
}


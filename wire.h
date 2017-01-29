#ifndef WIRE_H
#define WIRE_H

#include "component.h"
#include "circuitcanvas.h"
#include "node.h"

#include <QGraphicsItemGroup>

class Wire : public QGraphicsItemGroup
{
public:
    Wire(Component*, Component*, CircuitCanvas*);
    ~Wire();

    void findWireConnections();
    void updateWire();
    Component* getConnectedComponent(Component*);

    Component *componentOne, *componentTwo;

private:
    CircuitCanvas *scene;

    QGraphicsLineItem *wireOne, *wireTwo;
    int indexCompOne, indexCompTwo;
    bool LeftToRightOneTwo, BottomTopOneTwo;
};

#endif // WIRE_H

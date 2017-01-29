#ifndef COMPONENT_H
#define COMPONENT_H


#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>

#include <vector>

using namespace std;

class Wire;

class Component : public QGraphicsPixmapItem
{
public:
    string name;
    QGraphicsSimpleTextItem *nameLabel;
    bool isHorizontal;
    unsigned int gridSize, maximumNumberConnections;
    vector<Wire*> wires;
    vector< QPair<QPointF, Wire*> > connectionPoints;

    ~Component();
    void setOrientation(bool);
    void setSelection(bool);
    void initialiseConnectionPoints(int);
    void updateConnectionPoints();
    void addWire(Wire*);
    void removeWire(Wire*);
    bool checkConnected(Component*);
    Wire* getConnectedWire(Wire*);

protected:
    QPixmap horizontalPixmap, verticalPixmap, horizontalSelPixmap, verticalSelPixmap;

    explicit Component(string, QGraphicsItem *parent = 0);
    Component();
};

#endif // COMPONENT_H

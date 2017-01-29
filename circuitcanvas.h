#ifndef CIRCUITCANVAS_H
#define CIRCUITCANVAS_H


#include "component.h"
#include "node.h"

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QPointF>

class MainWindow;

class CircuitCanvas : public QGraphicsScene
{
    Q_OBJECT
public:
    int gridSize;
    explicit CircuitCanvas(MainWindow*, QObject *parent = 0);
    void resetSelectedObject();
protected:
   void drawBackground(QPainter * , const QRectF & );
   void mousePressEvent(QGraphicsSceneMouseEvent *);
   void mouseMoveEvent(QGraphicsSceneMouseEvent *);
   void mouseReleaseEvent(QGraphicsSceneMouseEvent *);
   void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *);

private:
   Component *object, *selectedObject;
   Node *checkNode;
   QPointF offset;
   MainWindow *mainWindow;

};

#endif // CIRCUITCANVAS_H

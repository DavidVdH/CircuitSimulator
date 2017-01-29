#include "circuitcanvas.h"
#include "component.h"
#include "mainwindow.h"
#include "edit_tab.h"
#include "run_tab.h"
#include "wire.h"

#include <cmath>
#include <QPen>
#include <QPainter>
#include <QMessageBox>
#include <QDebug>

using namespace std;

CircuitCanvas::CircuitCanvas(MainWindow *mainWindow, QObject *parent) :
    QGraphicsScene(parent)
{
    gridSize = 25; // Should also be manually changed in component.cpp
    this->mainWindow = mainWindow;
    object = 0;
    selectedObject = 0;
    checkNode = 0;
}


void CircuitCanvas::resetSelectedObject()
{
    if(selectedObject){
        selectedObject->setSelection(false);
        selectedObject = 0;
    }
}


//This function is overloaded to create a custom background with gridpoints
void CircuitCanvas::drawBackground(QPainter *painter, const QRectF &rect)
{
    // Specify color and size of gridpoints
    painter->setPen(QPen(Qt::lightGray,2));

    // Correct for offsets
    qreal left = int(rect.left()) - (int(rect.left()) % gridSize);
    qreal top = int(rect.top()) - (int(rect.top()) % gridSize);

    // Draw the points on the QGraphicsScene
    for (qreal x = left; x < rect.right(); x += gridSize){
        for (qreal y = top; y < rect.bottom(); y += gridSize){
            painter->drawPoint(x,y);
        }
    }
}


/*
    In what follows the drag and snap to grid is implemented by overloading
    mousePressEvent, mouseMoveEvent and mouseReleaseEvent; Additionally the
    mouseDoubleClickEvent is overloaded.
*/


// mousePressEvent stores the offset between items position and mouse position
void CircuitCanvas::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    object = dynamic_cast<Component*>(itemAt(event->scenePos(),QTransform()));
    // if there is no object at the event location itemAt() returns 0

    if(object){
        // calculate offset between top left corner of selected item and the mouse
        offset = event->scenePos() - object->pos();
    }else{
        // If there is no item at event location the default implementation
        // handles further bookkeeping (leads to ignoring event)
        QGraphicsScene::mousePressEvent(event);
    }
}


// mouseMoveEvent keeps offset between items position and mouse position
// constant during moving so the item moves togehter with the mouse
void CircuitCanvas::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(object){
        object->setPos(event->scenePos()-offset);
    }else{
        QGraphicsScene::mouseMoveEvent(event);
    }
}


// mouseReleaseEvent sets new position for the item. The item "snaps" to the grid.
void CircuitCanvas::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(object){
        int xPos, yPos;

        // calculate new position
        xPos = floor( (event->scenePos().x() / (2*gridSize))) * 2*gridSize;
        yPos = floor( (event->scenePos().y() / (2*gridSize))) * 2*gridSize;

        // set new position on grid
        object->setPos(xPos, yPos);

        // update coordinates of connection points
        Node *node = dynamic_cast<Node*>(object);
        if(node){
            node->updateConnectionPoints();
        }else{
            object->updateConnectionPoints();
        }

        // update wire positions
        for(unsigned int i = 0; i < object->wires.size(); i++){
            object->wires.at(i)->updateWire();
        }

        // reset and set ready for next event
        object = 0;
        offset.isNull();
    }else{
        QGraphicsScene::mouseReleaseEvent(event);
    }
}


// mouseDoubleClickEvent makes it possible to edit properties of the
// selected component in the "Edit" tab, makes it possible to connect
// components and in general to select a component.
void CircuitCanvas::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    object = dynamic_cast<Component*>(itemAt(event->scenePos(),QTransform()));

    if(object){

        // Implementation of behaviour when the 'connect' button is checked
        // and the first object has already been selected.
        if(mainWindow->edit_Tab->connectPushButton->isChecked() && selectedObject != 0){
            try{
                // if selectedObject is not object itself, if the maximum number of connections has not been exceeded yet
                // and if the objects are not already connected than the connection can be made.
                if(selectedObject == object){
                    throw "This component can not be connected to itself!";
                }else if(selectedObject->wires.size() >= selectedObject->maximumNumberConnections ||
                         object->wires.size() >= object->maximumNumberConnections ){
                    throw "One of the components already has the maximum number of connections!";
                }else if(object->checkConnected(selectedObject)){
                    throw "These components are already connected!";
                }else{
                    // Create new wire to connect both components
                    Wire *newWire = new Wire(selectedObject, object, this);
                    object->addWire(newWire);
                    selectedObject->addWire(newWire);
                }
            }catch(const char* e){
                QMessageBox msgBox;
                msgBox.setText(e);
                msgBox.exec();
            }
            selectedObject->setSelection(false);
            selectedObject = 0;

        // Implementation of behaviour when the 'connect' button is checked
        // but the first component has to be slected.
        }else if(mainWindow->edit_Tab->connectPushButton->isChecked() && selectedObject == 0){
            selectedObject = object;
            selectedObject->setSelection(true);

        // Implementation of behaviour when the 'run' button is checked.
        // The reference node is selected.
        }else if(mainWindow->run_Tab->runPushButton->isChecked()){
            checkNode = dynamic_cast<Node*>(object);
            try{
                if(mainWindow->run_Tab->referenceNode != NULL){
                    throw "A reference node has already been chosen.";
                }else if(!checkNode){
                    throw "The selected object is not a node, please try again.";
                }else if(checkNode){
                    mainWindow->run_Tab->setReferenceNode(checkNode);
                    selectedObject = object;
                    selectedObject->setSelection(true);
                }
            }catch(const char* e){
                QMessageBox msgBox;
                msgBox.setText(e);
                msgBox.exec();
            }
            checkNode = 0;

        // Implementation of default behaviour. When a component is double clicked
        // the properties can be modified on the 'edit' tab.
        }else if(!mainWindow->edit_Tab->connectPushButton->isChecked()){
            // if there is an object, switch to the edit tab
            mainWindow->tabMenu->setCurrentIndex(1);
            mainWindow->edit_Tab->receiveComponent(object);
        }
    }else{
        QGraphicsScene::mouseDoubleClickEvent(event);
    }

    object = 0;
}


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "component.h"
#include "circuitcanvas.h"

#include <vector>

#include <QWidget>
#include <QMainWindow>
#include <QGraphicsView>

class Add_Tab;
class Edit_Tab;
class Run_Tab;

using namespace std;

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    vector<Component*> componentList;

    //canvas widgets
    CircuitCanvas *scene;
    QGraphicsView *view;

    // Menu widgets
    QTabWidget *tabMenu;
    Add_Tab *add_Tab;
    Edit_Tab *edit_Tab;
    Run_Tab *run_Tab;

};

#endif // MAINWINDOW_H

#include "mainwindow.h"
#include "add_tab.h"
#include "edit_tab.h"
#include "run_tab.h"
#include "circuitcanvas.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{
    // Size of the main window ==> all other sizes relative to these
    int width = 1200;
    int height = 600;

    setFixedSize(width, height);

    // Setup menu
    tabMenu = new QTabWidget(this);
    tabMenu->setGeometry(20, 20, width*0.25, height-40);

    add_Tab = new Add_Tab(this);
    edit_Tab = new Edit_Tab(this);
    run_Tab = new Run_Tab(this);

    tabMenu->addTab(add_Tab, "Add");
    tabMenu->addTab(edit_Tab, "Edit");
    tabMenu->addTab(run_Tab, "Run");

    // Create circuit canvas
    scene = new CircuitCanvas(this);
    view = new QGraphicsView(scene, this);
    view->setGeometry(width*0.25+30, 50, width - width*0.25 - 45,height - 70);
    view->setSceneRect(0, 0, view->size().width()-5, view->size().height()-5);
}

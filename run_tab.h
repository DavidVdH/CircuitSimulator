#ifndef RUN_TAB_H
#define RUN_TAB_H

#include "tablayout.h"
#include "mainwindow.h"
#include "resistor.h"
#include "voltagesource.h"
#include "node.h"

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <vector>

using namespace std;

class Run_Tab : public QWidget, public TabLayout
{
    Q_OBJECT
public:
    explicit Run_Tab(MainWindow*, QWidget *parent = 0);
    QPushButton *runPushButton;
    QLabel *explanationLabel, *warningLabel, *waitingLabel;
    Node *referenceNode;

private:
    Resistor *resistor;
    VoltageSource *voltageSource;
    Node *node;
    vector<Node*> nodeList;
    vector<QGraphicsSimpleTextItem*> labelList;

public slots:
    void setReferenceNode(Node*);

private slots:
    void runCircuit();
    void calculateVoltages();
};

#endif // RUN_TAB_H

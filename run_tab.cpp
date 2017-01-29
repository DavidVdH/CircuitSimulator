#include "run_tab.h"
#include "component.h"
#include "wire.h"
#include "resistor.h"
#include "voltagesource.h"
#include "node.h"

#include <iostream>
#include <QDebug>
#include <Eigen/Dense> // provides matrices and vectors
#include <Eigen/LU> // provides matrix.inverse()

using namespace Eigen;
using namespace std;

Run_Tab::Run_Tab(MainWindow *mainWindow, QWidget *parent) :
    QWidget(parent), TabLayout(mainWindow, this)
{
    resistor=0;
    voltageSource=0;
    node = 0;
    referenceNode = 0;

    /*
        Setup of blank "Run" tab.
    */

    explanationLabel = new QLabel("Please make sure the circuit is closed before clicking the 'Run' button.", this);
    explanationLabel->setWordWrap(true);
    explanationLabel->setGeometry(10,10,280,60);

    runPushButton = new QPushButton("Run", this);
    runPushButton->setCheckable(true);
    runPushButton->setGeometry(10,80,280,30);

    warningLabel = new QLabel("Please select a reference node by double clicking on a node!", this);
    warningLabel->setWordWrap(true);
    warningLabel->setGeometry(10,130,280,40);

    QPalette* palette = new QPalette();
    palette->setColor(QPalette::WindowText,Qt::red); // needed to set label font color

    warningLabel->setPalette(*palette);
    warningLabel->setVisible(false);

    waitingLabel = new QLabel("The caluclations have begun. The results will appear shortly!", this);
    waitingLabel->setWordWrap(true);
    waitingLabel->setGeometry(10,130,280,40);

    palette->setColor(QPalette::WindowText,Qt::green);

    waitingLabel->setPalette(*palette);
    waitingLabel->setVisible(false);


    /*
        SIGNAL and SLOT connections.
    */

    QObject::connect(runPushButton, SIGNAL(toggled(bool)), SLOT(runCircuit()));
}

void Run_Tab::runCircuit()
{
    if(runPushButton->isChecked()){
        // Make sure the 'run' tab can not be left when run button is checked
        mainWindow->tabMenu->setTabEnabled(0,false);
        mainWindow->tabMenu->setTabEnabled(1,false);
        warningLabel->setVisible(true);

        // Make list containing all nodes in de circuit
        for(unsigned int i = 0; i < mainWindow->componentList.size(); i++){
            node = dynamic_cast<Node*>(mainWindow->componentList.at(i));
            if(node){
                nodeList.push_back(node);
            }
        }

        node = 0; // reset node to NULL for further use later on

    }else{
        // Make sure other tabs are enabled when run button is unchecked
        mainWindow->tabMenu->setTabEnabled(0,true);
        mainWindow->tabMenu->setTabEnabled(1,true);
        mainWindow->scene->resetSelectedObject();
        warningLabel->setVisible(false);
        waitingLabel->setVisible(false);

        referenceNode = 0; // reset reference node in case a second calculation is performed
        nodeList.clear();

        for(unsigned int i = 0; i < labelList.size(); i++){
            delete labelList.at(i); // remove labels with voltages when run button is unchecked
        }
        labelList.clear();
    }
}


// Sets a node as reference node. Called by mouseDoubleClickEvent of scene
void Run_Tab::setReferenceNode(Node *referenceNode)
{
    this->referenceNode = referenceNode;
    warningLabel->setVisible(false);
    waitingLabel->setVisible(true);
    calculateVoltages(); // Once reference node has been set, start calculations
}


// Function that calculates the voltages using Kirchoff's laws (node analysis).
// The voltages are found by inverting coefficients*V = constants ==> V = (coefficients.inverse)*constants
// "Eigen" is used as linear algebra package
// For now two nodes must be separated by at least one resistor!

void Run_Tab::calculateVoltages()
{
    // Dimensions of matrix are equal to the number of nodes
    MatrixXd coefficients(nodeList.size(), nodeList.size());
    VectorXd constants(nodeList.size());

    // Set initial values to 0
    coefficients.fill(0);
    constants.fill(0);

    double R, V;

    // setup of equation for each node in the circuit
    for(unsigned int i = 0; i < nodeList.size(); i++){

        int index = nodeList.at(i)->nodeId;

        if(nodeList.at(i) == referenceNode){
            // Voltage of reference node is set to 0
            coefficients(index, index) = 1;
            constants(index) = 0;
        }else{
            // Loop over all wires connected to the node
            for(unsigned int j = 0; j < nodeList.at(i)->wires.size(); j++){

                R = 0;
                V = 0;

                Component *component = nodeList.at(i);
                Wire *wire = nodeList.at(i)->wires.at(j);

                // Travel along circuit untill another node is found
                while(!node){
                    component = wire->getConnectedComponent(component); // find next component
                    node = dynamic_cast<Node*>(component);
                    resistor = dynamic_cast<Resistor*>(component);
                    voltageSource = dynamic_cast<VoltageSource*>(component);
                    if(resistor){
                        R += resistor->resistance;
                    }else if(voltageSource){
                        // sign depends on how the wire is connected to the source (+ or -)
                        if(voltageSource->isPositiveGate(wire)){
                            V += voltageSource->voltage;
                        }else{
                            V -=voltageSource->voltage;
                        }
                    }
                    wire = component->getConnectedWire(wire); // find next wire
                }

                // set coefficients and constants of matrix
                coefficients(index,index) += 1./R;
                coefficients(index, node->nodeId) -=1./R;
                constants(index) += V/R;

                // reset for next node/wire
                node = 0;
                resistor = 0;
                voltageSource = 0;
            }
        }
    }

    VectorXd voltages = coefficients.inverse()*constants; // calculate voltages

    // Display voltages next to every node
    for(unsigned int k = 0; k < nodeList.size(); k++){
        QGraphicsSimpleTextItem *voltageLabel = new QGraphicsSimpleTextItem(QString::number(voltages(nodeList.at(k)->nodeId)));
        voltageLabel->setBrush(QBrush(Qt::green));
        voltageLabel->setPos(nodeList.at(k)->scenePos());
        mainWindow->scene->addItem(voltageLabel);

        labelList.push_back(voltageLabel);
    }
}

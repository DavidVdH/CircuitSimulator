#include "add_tab.h"
#include "resistor.h"
#include "voltagesource.h"
#include "node.h"

#include <iostream>

using namespace std;

Add_Tab::Add_Tab(MainWindow *mainWindow, QWidget *parent) :
    QWidget(parent), TabLayout(mainWindow, this)
{
    this->mainWindow = mainWindow;

    /*
        Setup of blank "Add" tab.
    */
    addLabel = new QLabel("Element Type: ", this);
    addLabel->setGeometry(10,10,110,30);

    addComboBox = new QComboBox(this);
    addComboBox->addItem("None");
    addComboBox->addItem("Node");
    addComboBox->addItem("Resistor");
    addComboBox->addItem("Voltage Source");
    addComboBox->setGeometry(140,10,150,30);

    newPushButton = new QPushButton("New", this);
    newPushButton->setGeometry(10,50,280,30);

    cancelPushButton = new QPushButton("Cancel", this);
    cancelPushButton->setDisabled(true);
    cancelPushButton->setGeometry(10,490,130,30);

    savePushButton = new QPushButton("Save", this);
    savePushButton->setDisabled(true);
    savePushButton->setGeometry(150,490,130,30);

    /*
        Setup of component groupboxes on "Add" tab
    */

    generalGroupBox->setGeometry(10,100,280,150);
    resistorGroupBox->setGeometry(10,260,280,150);
    voltageGroupBox->setGeometry(10,260,280,150);


    /*
        SIGNAL and SLOT connections.
    */

    QObject::connect(newPushButton, SIGNAL(clicked()), SLOT(newComponent()));
    QObject::connect(cancelPushButton, SIGNAL(clicked()), SLOT(cancelComponent()));
    QObject::connect(savePushButton, SIGNAL(clicked()), SLOT(saveComponent()));
}


// Function that displays the necessary fields with properties of the selected component
// on the "add" tab and enables the "save" and "cancel" buttons.
void Add_Tab::newComponent()
{
    if(addComboBox->currentText().toStdString() == "None"){
        // do nothing
    }else{
        generalGroupBox->setVisible(true);

        if(addComboBox->currentText().toStdString() == "Resistor"){
            resistorGroupBox->setVisible(true);
        }else if(addComboBox->currentText().toStdString() == "Voltage Source"){
            voltageGroupBox->setVisible(true);
        }else if(addComboBox->currentText().toStdString() == "Node"){
            orientationComboBox->setDisabled(true);
        }

        addComboBox->setDisabled(true);
        newPushButton->setDisabled(true);
        cancelPushButton->setDisabled(false);
        savePushButton->setDisabled(false);
    }
}

// Function that resets all property fields to their default value.
// This can be used to cancel a component or after saveComponent()
// to ready the setup for the next component.
void Add_Tab::cancelComponent()
{
    // Hide the componentspecific groupboxes
    generalGroupBox->setVisible(false);
    resistorGroupBox->setVisible(false);
    voltageGroupBox->setVisible(false);


    // resetting general properties
    nameLineEdit->clear();
    xPosSpinBox->setValue(0);
    yPosSpinBox->setValue(0);
    orientationComboBox->setCurrentIndex(0);

    // resetting component specific properties
    RSpinBox->setValue(0);
    VSpinBox->setValue(0);

    orientationComboBox->setDisabled(false);
    addComboBox->setDisabled(false);
    newPushButton->setDisabled(false);
    cancelPushButton->setDisabled(true);
    savePushButton->setDisabled(true);
}

// Initiates a new component of type specified in the addComboBox
// with the properties from the property fields and adds it to the scene.
void Add_Tab::saveComponent()
{
    Component *newComp;

    // Make a difference between several different component types
    if(addComboBox->currentText().toStdString() == "Resistor"){
        if(orientationComboBox->currentText().toStdString() == "Horizontal"){
            newComp = new Resistor(nameLineEdit->text().toStdString(), true, RSpinBox->value());
        }else{
            newComp = new Resistor(nameLineEdit->text().toStdString(), false, RSpinBox->value());
        }
    }else if(addComboBox->currentText().toStdString() == "Voltage Source"){
        if(orientationComboBox->currentText().toStdString() == "Horizontal"){
            newComp = new VoltageSource(nameLineEdit->text().toStdString(), true, VSpinBox->value());
        }else{
            newComp = new VoltageSource(nameLineEdit->text().toStdString(), false, VSpinBox->value());
        }
    }else if(addComboBox->currentText().toStdString() == "Node"){
        newComp = new Node(nameLineEdit->text().toStdString());
    }

    // Set position of new component on scene and add it to the scene
    newComp->setPos(xPosSpinBox->value()*mainWindow->scene->gridSize, yPosSpinBox->value()*mainWindow->scene->gridSize);
    mainWindow->scene->addItem(newComp);

    // Update the coordinates of the connection points. This function is overloaded
    // for the Node so a difference is made.
    Node *node = dynamic_cast<Node*>(newComp);
    if(node){
        node->updateConnectionPoints();
    }else{
        newComp->updateConnectionPoints();
    }

    // Add component to componentList to keep track of it
    mainWindow->componentList.push_back(newComp);

    // Reset all the fields to default values using cancelComponent()
    cancelComponent();
}


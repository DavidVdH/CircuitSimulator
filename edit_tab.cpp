#include "edit_tab.h"
#include "resistor.h"
#include "voltagesource.h"
#include "node.h"
#include "wire.h"

#include <QDebug>

Edit_Tab::Edit_Tab(MainWindow *mainWindow, QWidget *parent) :
    QWidget(parent), TabLayout(mainWindow, this)
{
    this->mainWindow = mainWindow;

    resistor=0;
    voltageSource=0;
    node = 0;

    /*
        Setup of blank "Edit" tab.
    */
    explanationLabel = new QLabel("Double click on a component to modify it's properties. "
                                  "To connect components click the 'Connect' button"
                                  " and double click on both components.", this);
    explanationLabel->setWordWrap(true);
    explanationLabel->setGeometry(10,10,280,70);

    connectPushButton = new QPushButton("Connect", this);
    connectPushButton->setCheckable(true);
    connectPushButton->setGeometry(10,90,280,30);

    cancelPushButton = new QPushButton("Cancel", this);
    cancelPushButton->setDisabled(true);
    cancelPushButton->setGeometry(10,450,130,30);

    savePushButton = new QPushButton("Save", this);
    savePushButton->setDisabled(true);
    savePushButton->setGeometry(150,450,130,30);

    removePushButton = new QPushButton("Remove", this);
    removePushButton->setDisabled(true);
    removePushButton->setGeometry(10,490,270,30);

    /*
        Setup of component groupboxes on "Edit" tab
    */

    generalGroupBox->setGeometry(10,130,280,150);
    resistorGroupBox->setGeometry(10,290,280,150);
    voltageGroupBox->setGeometry(10,290,280,150);

    /*
        SIGNAL and SLOT connections.
    */

    QObject::connect(connectPushButton, SIGNAL(toggled(bool)), SLOT(connectComponent()));
    QObject::connect(cancelPushButton, SIGNAL(clicked()), SLOT(cancelComponent()));
    QObject::connect(savePushButton, SIGNAL(clicked()), SLOT(saveComponent()));
    QObject::connect(removePushButton, SIGNAL(clicked()),SLOT(removeComponent()));
}

// When an item is double clicked the fields are filled with the corresponding
// properties and the relevant groupbox with properties is displayed.
void Edit_Tab::receiveComponent(Component *object)
{
    cancelComponent(); // make sure all fields are cleared

    savePushButton->setDisabled(false);
    cancelPushButton->setDisabled(false);
    removePushButton->setDisabled(false);

    // Check wheter the component is a resistor, a voltage source or a node.
    resistor = dynamic_cast<Resistor*>(object);
    voltageSource = dynamic_cast<VoltageSource*>(object);
    node = dynamic_cast<Node*>(object);

    // Fill in property fields with corresponding values.
    if(resistor){

        nameLineEdit->setText(QString::fromStdString(resistor->name));
        xPosSpinBox->setValue(resistor->scenePos().x()/mainWindow->scene->gridSize);
        yPosSpinBox->setValue(resistor->scenePos().y()/mainWindow->scene->gridSize);
        RSpinBox->setValue(resistor->resistance);
        if(!resistor->isHorizontal){
            orientationComboBox->setCurrentIndex(1);
        }

        resistorGroupBox->setVisible(true);

    }else if(voltageSource){

        nameLineEdit->setText(QString::fromStdString(voltageSource->name));
        xPosSpinBox->setValue(voltageSource->scenePos().x()/mainWindow->scene->gridSize);
        yPosSpinBox->setValue(voltageSource->scenePos().y()/mainWindow->scene->gridSize);
        VSpinBox->setValue(voltageSource->voltage);
        if(!voltageSource->isHorizontal){
            orientationComboBox->setCurrentIndex(1);
        }

        voltageGroupBox->setVisible(true);

    }else if(node){

        nameLineEdit->setText(QString::fromStdString(node->name));

        xPosSpinBox->setValue(node->scenePos().x()/mainWindow->scene->gridSize);
        yPosSpinBox->setValue(node->scenePos().y()/mainWindow->scene->gridSize);

        orientationComboBox->setDisabled(true);
    }

    generalGroupBox->setVisible(true);
}


// When the connect button is toggled no other actions must be taken so
// the 'Add' and the 'Run' tab are disabled.
// They are enabled when connect button is clicked again.
void Edit_Tab::connectComponent()
{
    if(connectPushButton->isChecked()){
        mainWindow->tabMenu->setTabEnabled(0,false);
        mainWindow->tabMenu->setTabEnabled(2,false);
        cancelComponent();
    }else{
        mainWindow->tabMenu->setTabEnabled(0,true);
        mainWindow->tabMenu->setTabEnabled(2,true);
        mainWindow->scene->resetSelectedObject();
    }
}


// Resets all the property fields to their default values
void Edit_Tab::cancelComponent()
{
    generalGroupBox->setVisible(false);
    resistorGroupBox->setVisible(false);
    voltageGroupBox->setVisible(false);
    savePushButton->setDisabled(true);
    cancelPushButton->setDisabled(true);
    removePushButton->setDisabled(true);
    orientationComboBox->setDisabled(false);


    // resetting general properties
    nameLineEdit->clear();
    xPosSpinBox->setValue(0);
    yPosSpinBox->setValue(0);
    orientationComboBox->setCurrentIndex(0);

    // resetting component specific properties
    RSpinBox->setValue(0);
    VSpinBox->setValue(0);

    resistor = 0;
    voltageSource = 0;
    node = 0;
}

// Stores the properties from the property fields into the object's property fields.
void Edit_Tab::saveComponent()
{
    if(resistor){
        resistor->name = nameLineEdit->text().toStdString();
        resistor->nameLabel->setText(nameLineEdit->text());
        resistor->setPos((xPosSpinBox->value())*mainWindow->scene->gridSize, yPosSpinBox->value()*mainWindow->scene->gridSize);
        resistor->resistance = RSpinBox->value();

        if(orientationComboBox->currentText().toStdString() == "Horizontal" && !resistor->isHorizontal){
            resistor->setOrientation(true);
        }else if(orientationComboBox->currentText().toStdString() == "Vertical" && resistor->isHorizontal){
            resistor->setOrientation(false);
        }

        resistor->updateConnectionPoints();
        for(unsigned int i = 0; i < resistor->wires.size(); i++){
            resistor->wires.at(i)->updateWire();
        }

     }else if(voltageSource){
        voltageSource->name = nameLineEdit->text().toStdString();
        voltageSource->nameLabel->setText(nameLineEdit->text());
        voltageSource->setPos(xPosSpinBox->value()*mainWindow->scene->gridSize, yPosSpinBox->value()*mainWindow->scene->gridSize);
        voltageSource->voltage = VSpinBox->value();

        if(orientationComboBox->currentText().toStdString() == "Horizontal" && !voltageSource->isHorizontal){
            voltageSource->setOrientation(true);
        }else if(orientationComboBox->currentText().toStdString() == "Vertical" && voltageSource->isHorizontal){
            voltageSource->setOrientation(false);
        }

        voltageSource->updateConnectionPoints();
        for(unsigned int i = 0; i < voltageSource->wires.size(); i++){
            voltageSource->wires.at(i)->updateWire();
        }

    }else if(node){
        node->name = nameLineEdit->text().toStdString();
        node->nameLabel->setText(nameLineEdit->text());
        node->setPos(xPosSpinBox->value()*mainWindow->scene->gridSize, yPosSpinBox->value()*mainWindow->scene->gridSize);

        node->updateConnectionPoints();
        for(unsigned int i = 0; i < node->wires.size(); i++){
            node->wires.at(i)->updateWire();
        }
    }

    cancelComponent();
}


// Completely removes the selected component. The component's destructor
// takes care of removing the connected wires as well.
void Edit_Tab::removeComponent()
{
    vector<Component*>::iterator compIt;

    if(resistor){
        mainWindow->scene->removeItem(resistor);
        compIt = std::find(mainWindow->componentList.begin(), mainWindow->componentList.end(), resistor);
    }else if(voltageSource){
        mainWindow->scene->removeItem(voltageSource);
        compIt = std::find(mainWindow->componentList.begin(), mainWindow->componentList.end(), voltageSource);
    }else if(node){
        mainWindow->scene->removeItem(node);
        compIt = std::find(mainWindow->componentList.begin(), mainWindow->componentList.end(), node);

        // Lower (-1) the nodeId of every node with a nodeId higher than the nodeId of the removed node.
        Node *testNode = 0;
        for(unsigned int i = 0; i < mainWindow->componentList.size(); i++){
            testNode = dynamic_cast<Node*>(mainWindow->componentList.at(i));
            if(testNode && (node->nodeId <= testNode->nodeId)){
                testNode->nodeId -= 1;
            }
        }
    }

    delete mainWindow->componentList.at(compIt - mainWindow->componentList.begin());
    mainWindow->componentList.erase(compIt);

    cancelComponent();
}


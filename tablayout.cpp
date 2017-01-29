#include "tablayout.h"

TabLayout::TabLayout(MainWindow *mainWindow, QWidget *parent)
{
    this->mainWindow = mainWindow;

    /*
        Setup of component property fields
    */

    nameLineEdit = new QLineEdit(); // general property
    nameLineEdit->setPlaceholderText("Name");
    nameLineEdit->setMaxLength(2);

    xPosSpinBox = new QSpinBox(); // general property
    xPosSpinBox->setRange(0, 20);
    xPosSpinBox->setValue(0);

    yPosSpinBox = new QSpinBox(); // general property
    yPosSpinBox->setRange(0, 20);
    yPosSpinBox->setValue(0);

    orientationComboBox = new QComboBox(); // general property
    orientationComboBox->addItem("Horizontal");
    orientationComboBox->addItem("Vertical");

    RSpinBox = new QSpinBox(); // resistor property
    RSpinBox->setRange(0,200000);

    VSpinBox = new QSpinBox(); // voltage source property
    VSpinBox->setRange(0,500);


    /*
        Creating component specific layouts
    */

    // 0: GENERAL
    generalFormLayout = new QFormLayout();
    generalFormLayout->addRow("Name: ", nameLineEdit);
    generalFormLayout->addRow("X coord.: ", xPosSpinBox);
    generalFormLayout->addRow("Y coord.: ", yPosSpinBox);
    generalFormLayout->addRow("Orientation: ", orientationComboBox);

    generalGroupBox = new QGroupBox("General Properties", parent);
    generalGroupBox->setLayout(generalFormLayout);
    generalGroupBox->setVisible(false);

    // 1: RESISTOR
    resistorFormLayout = new QFormLayout();
    resistorFormLayout->addRow("Resistance:  " , RSpinBox);

    resistorGroupBox = new QGroupBox("Specific properties", parent);
    resistorGroupBox->setLayout(resistorFormLayout);
    resistorGroupBox->setVisible(false);

    // 2: VOLTAGE SOURCE
    voltageFormLayout = new QFormLayout();
    voltageFormLayout->addRow("Voltage:        ", VSpinBox);

    voltageGroupBox = new QGroupBox("Specific properties", parent);
    voltageGroupBox->setLayout(voltageFormLayout);
    voltageGroupBox->setVisible(false);
}

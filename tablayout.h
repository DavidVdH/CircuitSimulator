#ifndef TABLAYOUT_H
#define TABLAYOUT_H

#include "mainwindow.h"

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QGroupBox>
#include <QGraphicsItem>

using namespace std;

class TabLayout
{
protected:
    MainWindow *mainWindow;
    QFormLayout *generalFormLayout, *resistorFormLayout, *voltageFormLayout;
    QGroupBox *generalGroupBox, *resistorGroupBox, *voltageGroupBox;
    QLineEdit *nameLineEdit;
    QSpinBox *xPosSpinBox, *yPosSpinBox, *RSpinBox, *VSpinBox;
    QComboBox *orientationComboBox;

public:
    TabLayout(MainWindow*, QWidget*);
};

#endif // TABLAYOUT_H

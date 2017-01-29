#ifndef EDIT_TAB_H
#define EDIT_TAB_H

#include "mainwindow.h"
#include "resistor.h"
#include "voltagesource.h"
#include "tablayout.h"
#include "node.h"

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QGraphicsItem>

#include <vector>

using namespace std;

class Edit_Tab : public QWidget, public TabLayout
{
    Q_OBJECT
public:
    explicit Edit_Tab(MainWindow*, QWidget *parent = 0);
    QPushButton *connectPushButton;
private:
    Resistor *resistor;
    VoltageSource *voltageSource;
    Node *node;
    QLabel *explanationLabel;
    QPushButton *cancelPushButton, *savePushButton, *removePushButton;


public slots:
    void receiveComponent(Component*);

private slots:
    void connectComponent();
    void cancelComponent();
    void saveComponent();
    void removeComponent();

};

#endif // EDIT_TAB_H

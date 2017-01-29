#ifndef ADD_TAB_H
#define ADD_TAB_H

#include "mainwindow.h"
#include "tablayout.h"

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>

#include <vector>

using namespace std;


class Add_Tab : public QWidget, public TabLayout
{
    Q_OBJECT
public:
    explicit Add_Tab(MainWindow* , QWidget *parent = 0);

private:
    QLabel *addLabel;
    QComboBox *addComboBox;
    QPushButton *newPushButton, *cancelPushButton, *savePushButton;

private slots:
    void newComponent();
    void cancelComponent();
    void saveComponent();
};

#endif // ADD_TAB_H

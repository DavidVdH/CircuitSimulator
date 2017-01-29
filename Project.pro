TEMPLATE = app
TARGET = Circuit

QT = core gui
INCLUDEPATH += /home/david/Eigen/

greaterThan(QT_MAJOR_VERSION, 4):QT += widgets

HEADERS += \
    add_tab.h \
    mainwindow.h \
    edit_tab.h \
    tablayout.h \
    resistor.h \
    voltagesource.h \
    node.h \
    wire.h \
    run_tab.h \
    component.h \
    circuitcanvas.h

SOURCES += \
    main.cpp \
    add_tab.cpp \
    mainwindow.cpp \
    edit_tab.cpp \
    tablayout.cpp \
    resistor.cpp \
    voltagesource.cpp \
    node.cpp \
    wire.cpp \
    run_tab.cpp \
    component.cpp \
    circuitcanvas.cpp


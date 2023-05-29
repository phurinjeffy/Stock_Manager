QT       += core gui sql charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Stock_Manager
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++17

SOURCES += \
    clothingitem.cpp \
    deleteitem.cpp \
    electronicsitem.cpp \
    furnitureitem.cpp \
    graph.cpp \
    item.cpp \
    itemhandle.cpp \
        main.cpp \
        mainwindow.cpp \
    additem.cpp \
    perishableitem.cpp \
    reserveditems.cpp \
    transaction.cpp \
    updateitem.cpp \
    itemsreport.cpp \
    mydb.cpp \
    solditems.cpp \
    expireditems.cpp \
    returneditems.cpp \
    availableitems.cpp \
    finditem.cpp

HEADERS += \
    clothingitem.h \
    deleteitem.h \
    electronicsitem.h \
    furnitureitem.h \
    graph.h \
    item.h \
    itemhandle.h \
        mainwindow.h \
    additem.h \
    perishableitem.h \
    reserveditems.h \
    transaction.h \
    updateitem.h \
    itemsreport.h \
    mydb.h \
    solditems.h \
    expireditems.h \
    returneditems.h \
    availableitems.h \
    finditem.h

FORMS += \
    deleteitem.ui \
    graph.ui \
        mainwindow.ui \
    additem.ui \
    reserveditems.ui \
    transaction.ui \
    updateitem.ui \
    itemsreport.ui \
    solditems.ui \
    expireditems.ui \
    returneditems.ui \
    availableitems.ui \
    finditem.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

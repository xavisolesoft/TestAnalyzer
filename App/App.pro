QT       += core gui widgets


CONFIG += c++17
DEFINES += QT_DEPRECATED_WARNINGS

include(../GTest/GTest.pri)
include(../TestModel/TestModel.pri)

SOURCES += \
    TestTableModel.cpp \
    TestTableView.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    MainWindow.h \
    TestTableModel.hpp \
    TestTableView.hpp

FORMS += \
    MainWindow.ui

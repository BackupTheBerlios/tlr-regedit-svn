TEMPLATE = app
CONFIG += qt debug thread 
TARGET = regedit
FORMS += mainwidget.ui newdialog.ui
HEADERS += newkeydialogimpl.h mainwindowimpl.h
SOURCES += newkeydialogimpl.cpp mainwindowimpl.cpp main.cpp
LIBS += -lregistry

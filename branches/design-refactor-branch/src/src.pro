# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterverzeichnis relativ zum Projektverzeichnis: ./src
# Das Target ist eine Anwendung:  regedit

LIBS += $$system(pkg-config --libs elektra) 
TARGET = kdbe 
CONFIG += debug \
          warn_on \
          qt \
          thread 
TEMPLATE = app 
FORMS += mainwidget.ui \
         newdialog.ui \
         permissiondialog.ui 
HEADERS += newkeydialogimpl.h \
           mainwidgetimpl.h \
           mainwindowimpl.h \
           regedit_globals.h \
           permissiondialogimpl.h \
           command.h \
           keyremovecommand.h \
           keymodifycommand.h \
           keyaddcommand.h 
SOURCES += newkeydialogimpl.cpp \
           main.cpp \
           mainwidgetimpl.cpp \
           mainwindowimpl.cpp \
           regedit_globals.cpp \
           permissiondialogimpl.cpp \
           command.cpp \
           keyremovecommand.cpp \
           keymodifycommand.cpp \
           keyaddcommand.cpp 

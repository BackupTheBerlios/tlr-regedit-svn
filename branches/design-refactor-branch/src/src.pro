# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterverzeichnis relativ zum Projektverzeichnis: ./src
# Das Target ist eine Anwendung:  kdbe

FORMS += editorviewui.ui \
         newkeywizardui.ui 
HEADERS += editorview.h \
           editorcontroller.h \
           keymetainfo.h \
           newkeywizard.h \
           command.h \
           keyaddcommand.h \
           keymodifycommand.h \
           keyremovecommand.h 
SOURCES += main.cpp \
           editorview.cpp \
           editorcontroller.cpp \
           keymetainfo.cpp \
           newkeywizard.cpp \
           command.cpp \
           keyaddcommand.cpp \
           keymodifycommand.cpp \
           keyremovecommand.cpp 
LIBS += $$system(pkg-config --libs elektra) -lmagic -L./xdgmime/ -lxdgmime
TARGET = kdbe
CONFIG += debug \
warn_on \
qt \
thread
TEMPLATE = app subdirs

kdbe.path = /usr/bin/
kdbe.files = kdbe

INSTALLS += kdbe

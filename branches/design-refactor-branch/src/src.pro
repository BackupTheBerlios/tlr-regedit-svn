# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterverzeichnis relativ zum Projektverzeichnis: ./src
# Das Target ist eine Anwendung:  regedit

LIBS += $$system(pkg-config --libs elektra) -lmagic -L./xdgmime/ -lxdgmime
TARGET = kdbe 
CONFIG += debug \
          warn_on \
          qt \
          thread 
TEMPLATE = app subdirs 
FORMS += editorviewui.ui \
	newkeywizardui.ui
      
HEADERS += editorview.h \
           editorcontroller.h \
	   keymetainfo.h \
	   newkeywizard.h

SOURCES += main.cpp \
           editorview.cpp \
           editorcontroller.cpp \
	   keymetainfo.cpp \
	   newkeywizard.cpp

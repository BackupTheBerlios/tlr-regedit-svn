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
FORMS += editorviewui.ui
      
HEADERS += editorview.h \
           editorcontroller.h \
	   keymetainfo.h

SOURCES += main.cpp \
           editorview.cpp \
           editorcontroller.cpp \
	   keymetainfo.cpp
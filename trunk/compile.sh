#!/bin/sh

##############################################
##
## Script to workaround the need of cmake.
## Must work for everybody having QT
## installed somewhere
##
## Avi Alkalay <avi A/T unix.sh>
## 20 Jul 2004
##

QTDIR=/usr/lib/qt-3.3
QTLIB=qt-mt

set -vx

uic -tr QObject::tr mainwidget.ui > mainwidget.h
uic -tr QObject::tr -impl mainwidget.h mainwidget.ui > mainwidget.cpp

uic -tr QObject::tr newdialog.ui > newdialog.h
uic -tr QObject::tr -impl newdialog.h newdialog.ui > newdialog.cpp

moc mainwindowimpl.h > mainwindowimpl.moc.cpp
moc mainwidget.h > mainwidget.moc.cpp
moc newkeydialogimpl.h > newkeydialogimpl.moc.cpp
moc newdialog.h > newdialog.moc.cpp

cc -I . -I $QTDIR/include -L $QTDIR/lib/ -L /lib  -l $QTLIB -lregistry -o regedit main.cpp mainwindowimpl.cpp mainwidget.cpp mainwindowimpl.moc.cpp mainwidget.moc.cpp newkeydialogimpl.moc.cpp newkeydialogimpl.cpp newdialog.moc.cpp newdialog.cpp


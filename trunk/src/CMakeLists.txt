QT_WRAP_UI(regedit MWHEADERS MWCLASSES mainwidget.ui)
QT_WRAP_CPP(regedit MWMOCS mainwindowimpl.h mainwidgetimpl.h)

QT_WRAP_UI(regedit NKHEADERS NKCLASSES newdialog.ui)
QT_WRAP_CPP(regedit NKMOCS newkeydialogimpl.h)

QT_WRAP_UI(regedit PDHEADERS PDCLASSES permissiondialog.ui)
QT_WRAP_CPP(regedit PDMOCS permissiondialogimpl.h)

ADD_EXECUTABLE(regedit main.cpp mainwindowimpl.cpp mainwidgetimpl.cpp newkeydialogimpl.cpp permissiondialogimpl.cpp regedit_globals.cpp MWCLASSES MWMOCS NKCLASSES NKMOCS PDCLASSES PDMOCS)
TARGET_LINK_LIBRARIES(regedit registry ${QT_LIBRARIES})


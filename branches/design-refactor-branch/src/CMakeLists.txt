QT_WRAP_UI(regedit MWHEADERS MWCLASSES mainwidget.ui)
QT_WRAP_CPP(regedit MWMOCS mainwindowimpl.h mainwidgetimpl.h)

QT_WRAP_UI(regedit NKHEADERS NKCLASSES newdialog.ui)
QT_WRAP_CPP(regedit NKMOCS newkeydialogimpl.h)

QT_WRAP_UI(regedit PDHEADERS PDCLASSES permissiondialog.ui)
QT_WRAP_CPP(regedit PDMOCS permissiondialogimpl.h)

QT_WRAP_CPP(regedit CMDMOCS command.h)
QT_WRAP_CPP(regedit RMMOCS keyremovecommand.h)
QT_WRAP_CPP(regedit ADMOCS keyaddcommand.h)
QT_WRAP_CPP(regedit MDMOCS keymodifycommand.h)

SET(SOURCES main.cpp mainwindowimpl.cpp mainwidgetimpl.cpp newkeydialogimpl.cpp permissiondialogimpl.cpp regedit_globals.cpp command.cpp keyaddcommand.cpp keymodifycommand.cpp keyremovecommand.cpp)

SET(GEN_SOURCES ${MWCLASSES} ${MWMOCS} ${NKCLASSES} ${NKMOCS} ${PDCLASSES} ${PDMOCS} ${CMDMOCS} ${RMMOCS} ${MDMOCS} ${ADMOCS})

ADD_EXECUTABLE(regedit ${SOURCES} ${GEN_SOURCES})
TARGET_LINK_LIBRARIES(regedit registry ${QT_LIBRARIES})


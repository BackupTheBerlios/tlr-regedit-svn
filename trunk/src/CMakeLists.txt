QT_WRAP_UI(regedit MWHEADERS MWCLASSES mainwidget.ui)
QT_WRAP_CPP(regedit MWMOCS mainwindowimpl.h mainwidgetimpl.h)

QT_WRAP_UI(regedit NKHEADERS NKCLASSES newdialog.ui)
QT_WRAP_CPP(regedit NKMOCS newkeydialogimpl.h)

ADD_EXECUTABLE(regedit main.cpp mainwindowimpl.cpp mainwidgetimpl.cpp newkeydialogimpl.cpp regedit_globals.cpp MWCLASSES MWMOCS NKCLASSES NKMOCS)
TARGET_LINK_LIBRARIES(regedit registry ${QT_LIBRARIES})

ADD_EXECUTABLE(list list.cpp)
TARGET_LINK_LIBRARIES(list registry)

ADD_EXECUTABLE(test test.cpp)
TARGET_LINK_LIBRARIES(test registry)


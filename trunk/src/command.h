//
// C++ Interface: command
//
// Description: 
//
//
// Author: Gregor Burger <gregor.burger@aon.at>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef COMMAND_H
#define COMMAND_H

#include <qobject.h>

/**
@author Gregor Burger
*/
class MainWidgetImpl;

class Command : public QObject
{
Q_OBJECT
public:
	Command(MainWidgetImpl *mainWidget, const char *name = 0);
	virtual ~Command();
	MainWidgetImpl *mainWidget();
signals:
	void commandPerformed();
	
public slots:
	virtual void execute() = 0;
	virtual void unexecute() = 0;
private:
	MainWidgetImpl *m_mainWidget;
};

#endif

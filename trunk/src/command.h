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
class Command : public QObject
{
Q_OBJECT
public:
	Command(QObject *parent = 0, const char *name = 0);
	virtual ~Command();
signals:
	void commandPerformed();
	
public slots:
	virtual void execute() = 0;
	virtual void unexecute() = 0;
};

#endif

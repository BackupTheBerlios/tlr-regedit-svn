//
// C++ Implementation: command
//
// Description: 
//
//
// Author: Gregor Burger <gregor.burger@aon.at>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "command.h"
#include "mainwidgetimpl.h"

Command::Command(MainWidgetImpl *mainWidget, const char *name) : QObject (mainWidget, name)
{

}


Command::~Command()
{

}

MainWidgetImpl * Command::mainWidget()
{
	return m_mainWidget;
}



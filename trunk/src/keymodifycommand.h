//
// C++ Interface: keymodifycommand
//
// Description: 
//
//
// Author: Gregor Burger <gregor.burger@aon.at>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KEYMODIFYCOMMAND_H
#define KEYMODIFYCOMMAND_H

#include "command.h"

extern "C"
{
	#include <registry.h>
}
/**
@author Gregor Burger
*/
class KeyModifyCommand : public Command
{
Q_OBJECT
public:
    KeyModifyCommand(::Key *oldKey, ::Key *newKey, MainWidgetImpl *mainWidget, const char *name = 0);

    ~KeyModifyCommand();
    
public slots:
	virtual bool execute();
	virtual bool unexecute();

private:
	::Key *oldKey;
	::Key *newKey;
};

#endif

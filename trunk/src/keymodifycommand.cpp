//
// C++ Implementation: keymodifycommand
//
// Description: 
//
//
// Author: Gregor Burger <gregor.burger@aon.at>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "keymodifycommand.h"
#include "mainwidgetimpl.h"

KeyModifyCommand::KeyModifyCommand(::Key *oKey, ::Key *nKey, MainWidgetImpl *mainWidget, const char *name)
 : Command(mainWidget, name)
{
	oldKey = new ::Key;
	newKey = new ::Key;
	keyDup(oKey, oldKey);
	keyDup(nKey, newKey);
}


KeyModifyCommand::~KeyModifyCommand()
{
	keyClose(oldKey);
	delete oldKey;
	
	keyClose(newKey);
	delete newKey;
}

void KeyModifyCommand::execute()
{
	registryOpen();
	registrySetKey(newKey);
	registryClose();
	emit commandPerformed();
}

void KeyModifyCommand::unexecute()
{
	registryOpen();
	registrySetKey(oldKey);
	registryClose();
	emit commandPerformed();
}



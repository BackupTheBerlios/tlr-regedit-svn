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

KeyModifyCommand::KeyModifyCommand(::Key *oKey, ::Key *nKey, QObject *parent, const char *name)
 : Command(parent, name)
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



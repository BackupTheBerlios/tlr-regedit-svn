//
// C++ Implementation: addkeycommand
//
// Description: 
//
//
// Author: Gregor Burger <gregor.burger@aon.at>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "keyaddcommand.h"

KeyAddCommand::KeyAddCommand(::Key *key, MainWidgetImpl *mainWidget, const char *name)
 : Command(mainWidget, name)
{
	keyDup(key, addedKey);
}


KeyAddCommand::~KeyAddCommand()
{
	delete addedKey;
}

void KeyAddCommand::execute()
{
	registryOpen();
	registrySetKey(addedKey);
	registryClose();
	emit commandPerformed();
}

void KeyAddCommand::unexecute()
{
	registryOpen();
	char *name = new char[keyGetNameSize(addedKey)];
	keyGetName(addedKey, name, keyGetNameSize(addedKey));
	registryRemove(name);
	delete name;
	registryClose();
	emit commandPerformed();
}

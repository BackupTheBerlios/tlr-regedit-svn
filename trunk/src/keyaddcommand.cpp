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

KeyAddCommand::KeyAddCommand(::Key *key, QObject *parent, const char *name)
 : Command(parent, name)
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
}

void KeyAddCommand::unexecute()
{
	registryOpen();
	char *name = new char[keyGetNameSize(addedKey)];
	keyGetName(addedKey, name, keyGetNameSize(addedKey));
	registryRemove(name);
	delete name;
	registryClose();
}

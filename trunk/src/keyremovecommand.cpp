//
// C++ Implementation: keyremovecommand
//
// Description: 
//
//
// Author: Gregor Burger <gregor.burger@aon.at>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "keyremovecommand.h"

KeyRemoveCommand::KeyRemoveCommand(::Key *removedKey, MainWidgetImpl *parent, const char *name)
 : Command(parent, name)
{
	keyDup(removedKey, removed);
}


KeyRemoveCommand::~KeyRemoveCommand()
{
	keyClose(removed);
	delete(removed);
}

void KeyRemoveCommand::execute( )
{
	registryOpen();
	char *name = new char[keyGetNameSize(removed)];
	keyGetName(removed, name, keyGetNameSize(removed));
	registryRemove(name);
	delete name;
	registryClose();
	emit commandPerformed();
}

void KeyRemoveCommand::unexecute( )
{
	registryOpen();
	registrySetKey(removed);
	registryClose();
	emit commandPerformed();
}



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
#include "mainwidgetimpl.h"

#include <qlistview.h>

KeyRemoveCommand::KeyRemoveCommand(MainWidgetImpl *mainWidget, const char *name)
 : Command(mainWidget, name), item(mainWidget->keyTree->currentItem())
{
	key = new ::Key;
	keyDup(mainWidget->getSelected(), key);
}


KeyRemoveCommand::~KeyRemoveCommand()
{
	keyClose(key);
	delete key;
	delete item;
}

bool KeyRemoveCommand::execute( )
{
	registryOpen();
	
	char *name = new char[keyGetNameSize(key)];
	keyGetName(key, name, keyGetNameSize(key));
	
	if (registryRemove(name))
	{
		mainWidget()->showInStatusBar(strerror(errno));
		return false;
	}
	
	item->setVisible(false);
	registryClose();
	return true;
}

bool KeyRemoveCommand::unexecute( )
{
	registryOpen();
	if (registrySetKey(key))
	{
		mainWidget()->showInStatusBar(strerror(errno));
		return false;
	}
	
	item->setVisible(true);
	registryClose();
	return true;
}



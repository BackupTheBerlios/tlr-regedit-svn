/**
 *Copyright (C) 2004 gregor burger <gregor.burger@aon.at>
 *
 *This program is free software; you can redistribute it and/or
 *modify it under the terms of the GNU General Public License
 *as published by the Free Software Foundation; either version 2
 *of the License, or (at your option) any later version.
 *
 *This program is distributed in the hope that it will be useful,
 *but WITHOUT ANY WARRANTY; without even the implied warranty of
 *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *GNU General Public License for more details.
 *
 *You should have received a copy of the GNU General Public License
 *along with this program; if not, write to the Free Software
 *Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */

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
	kdbOpen();
		
	char *name = new char[keyGetNameSize(key)];
	keyGetName(key, name, keyGetNameSize(key));
	
	if (kdbRemove(name))
	{
		mainWidget()->showInStatusBar(strerror(errno));
		return false;
	}
	
	item->setVisible(false);
	kdbClose();
	return true;
}

bool KeyRemoveCommand::unexecute( )
{
	kdbOpen();
	if (kdbSetKey(key))
	{
		mainWidget()->showInStatusBar(strerror(errno));
		return false;
	}
	
	item->setVisible(true);
	kdbClose();
	return true;
}



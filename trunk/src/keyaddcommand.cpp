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

#include "keyaddcommand.h"
#include "mainwidgetimpl.h"
#include "newkeydialogimpl.h"

#include <errno.h>
#include <string.h>
#include <qlistview.h>

#include <iostream>
using namespace std;


KeyAddCommand::KeyAddCommand(NewKeyDialogImpl *dialog, MainWidgetImpl *mainWidget, const char *name)
 : Command(mainWidget, name), newDialog(dialog)
{
	key = new ::Key;
	keyInit(key);
		
	keySetType(key, newDialog->getType());
		
	if (newDialog->getName().length())
		keySetName(key, strdup(newDialog->getName()));
		
	if (newDialog->getComment().length())
		keySetComment(key, strdup(newDialog->getComment()));
	
	item = new QListViewItem(mainWidget->keyTree->currentItem(), newDialog->getName().section(RG_KEY_DELIM, -1));
	item->setVisible(false);
		
	int vlength = newDialog->getValue().length();
	
	switch (newDialog->getType())
	{
		case RG_KEY_TYPE_STRING:
			item->setPixmap(0, mainWidget->stringIcon);
			if (vlength) keySetString(key, strdup(newDialog->getValue()));
			break;
		case RG_KEY_TYPE_BINARY:
			item->setPixmap(0, mainWidget->binaryIcon);
			if (vlength) keySetBinary(key, strdup(newDialog->getValue()), vlength);
			break;
		case RG_KEY_TYPE_LINK:
			item->setPixmap(0, mainWidget->linkOverlay);
			if (vlength) keySetLink(key, strdup(newDialog->getValue()));
			break;
		case RG_KEY_TYPE_DIR:
			item->setPixmap(0, mainWidget->dirIcon);
			break;
	}
}


KeyAddCommand::~KeyAddCommand()
{
	registryOpen();
	keyClose(key);
	delete key;
	delete item;
	registryClose();
}

bool KeyAddCommand::execute()
{
	registryOpen();
	if (registrySetKey(key))
	{
		mainWidget()->showInStatusBar(strerror(errno));
		return false;
	}
	else
	{
		item->setVisible(true);
		//mainWidget()->keyTree->setSelected(item, true);
		return true;
	}
	registryClose();
}

bool KeyAddCommand::unexecute()
{
	registryOpen();
	char *name = new char[keyGetNameSize(key)];
	keyGetName(key, name, keyGetNameSize(key));
	if (registryRemove(name))
	{
		mainWidget()->showInStatusBar(strerror(errno));
		return false;
	}
	else
	{
		item->setVisible(false);
		return true;
		
	}
	registryClose();
}

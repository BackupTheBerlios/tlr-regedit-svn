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
	father = mainWidget->keyTree->currentItem();
}


KeyAddCommand::~KeyAddCommand()
{
	keyClose(key);
	delete key;
	delete item;
}

bool KeyAddCommand::execute()
{
	if (kdbSetKey(key))
	{
		mainWidget()->showInStatusBar(strerror(errno));
		return false;
	}
	else
	{
		item = genItemForKey();
		//mainWidget()->keyTree->setSelected(item, true);
		kdbSetKey(key);
		return true;
	}
}

bool KeyAddCommand::unexecute()
{
	char *name = new char[keyGetNameSize(key)];
	keyGetName(key, name, keyGetNameSize(key));
	if (kdbRemove(name))
	{
		mainWidget()->showInStatusBar(strerror(errno));
		return false;
	}
	else
	{
		delete item;
		kdbRemove(key->key);
		//item->setVisible(false);
		return true;
		
	}
}

QListViewItem *KeyAddCommand::genItemForKey()
{
	QListViewItem *temp = new QListViewItem(father, newDialog->getName().section(RG_KEY_DELIM, -1));
	//temp->setVisible(false);
		
	int vlength = newDialog->getValue().length();
	
	switch (newDialog->getType())
	{
		case KEY_TYPE_STRING:
			temp->setPixmap(0, mainWidget()->stringIcon);
			if (vlength) keySetString(key, strdup(newDialog->getValue()));
			break;
		case KEY_TYPE_BINARY:
			temp->setPixmap(0, mainWidget()->binaryIcon);
			if (vlength) keySetBinary(key, strdup(newDialog->getValue()), vlength);
			break;
		case KEY_TYPE_LINK:
			temp->setPixmap(0, mainWidget()->linkOverlay);
			if (vlength) keySetLink(key, strdup(newDialog->getValue()));
			break;
		case KEY_TYPE_DIR:
			temp->setPixmap(0, mainWidget()->dirIcon);
			break;
	}
	return temp;
}

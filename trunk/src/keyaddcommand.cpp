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
	cout << "servus" << endl;
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

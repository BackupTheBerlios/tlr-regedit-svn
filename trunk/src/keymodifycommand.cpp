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

#include <qlistview.h>
#include <qpixmap.h>
#include <qtextedit.h>
#include <qlineedit.h>
#include <qcombobox.h>

#include <iostream>
using namespace std;

KeyModifyCommand::KeyModifyCommand(MainWidgetImpl *mainWidget, const char *name)
 : Command(mainWidget, name), item(mainWidget->keyTree->currentItem())
{
	registryOpen();
	
	::Key *selected = this->mainWidget()->getSelected();
	
	oldKey = new ::Key;
	keyInit(oldKey);
	keyDup(selected,  oldKey);
	
	newKey = new ::Key;
	keyInit(newKey);
	keyDup(selected, newKey);
	
	oldIcon = *item->pixmap(0);
	
	char *comment = strdup(mainWidget->keyComment->text());
	keySetComment(newKey, comment);
	
	char *value = strdup(mainWidget->keyValue->text());
	keySetString(newKey, value);
	
	keySetAccess(newKey, mainWidget->getSelectedAccess());
	
	keySetType(newKey, mainWidget->types[mainWidget->typeCombo->currentItem()]);
	
	switch (mainWidget->typeCombo->currentItem())
	{
		case COMBO_POS_UND:
			//keyTree->currentItem()->setPixmap(0, undefinedIcon);
			break;
		case COMBO_POS_DIR:
			newIcon = mainWidget->dirIcon;
			break;
		case COMBO_POS_LNK:
			newIcon = mainWidget->linkOverlay;
			break;
		case COMBO_POS_STR:
			newIcon = mainWidget->stringIcon;
			break;
		case COMBO_POS_BIN:
			newIcon = mainWidget->binaryIcon;
			break;
	}
	
	registryClose();
}


KeyModifyCommand::~KeyModifyCommand()
{
	keyClose(oldKey);
	delete oldKey;
	
	keyClose(newKey);
	delete newKey;
}

bool KeyModifyCommand::execute()
{
	registryOpen();
	cout << "sel" << mainWidget()->getSelected() << endl;
	if (registrySetKey(newKey))
	{
		mainWidget()->showInStatusBar(strerror(errno));
		mainWidget()->showKeyValues(true);
		return false;
	}
	cout << "sel" << mainWidget()->getSelected() << endl;
	item->setPixmap(0, newIcon);
	cout << "sel" << mainWidget()->getSelected() << endl;
	mainWidget()->changeSelected(item);
	return true;
}

bool KeyModifyCommand::unexecute()
{
	registryOpen();
	if (registrySetKey(oldKey))
	{
		mainWidget()->showInStatusBar(strerror(errno));
		mainWidget()->showKeyValues(true);
		return false;
	}
	item->setPixmap(0, oldIcon);
	mainWidget()->changeSelected(item);
	registryClose();
	return true;
}



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
	kdbOpen();
	
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
	
	kdbClose();
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
	kdbOpen();
	if (kdbSetKey(newKey))
	{
		mainWidget()->showInStatusBar(strerror(errno));
		mainWidget()->showKeyValues(true);
		return false;
	}
	item->setPixmap(0, newIcon);
	mainWidget()->changeSelected(item);
	return true;
}

bool KeyModifyCommand::unexecute()
{
	kdbOpen();
	if (kdbSetKey(oldKey))
	{
		mainWidget()->showInStatusBar(strerror(errno));
		mainWidget()->showKeyValues(true);
		return false;
	}
	item->setPixmap(0, oldIcon);
	mainWidget()->changeSelected(item);
	kdbClose();
	return true;
}



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

#include <iostream>
using namespace std;

KeyRemoveCommand::KeyRemoveCommand(MainWidgetImpl *mainWidget, const char *name)
 : Command(mainWidget, name), icon(*mainWidget->keyTree->currentItem()->pixmap(0))
{
	key = new ::Key;
	this->name = strdup(mainWidget->keyTree->currentItem()->text(0));
	cout << "deleting " << this->name << endl;
	
	father = strdup(mainWidget->keyTree->currentItem()->parent()->text(0));
	keyInit(key);
	keySetName(key, mainWidget->getSelected());
	kdbGetKey(key);
}


KeyRemoveCommand::~KeyRemoveCommand()
{
	keyClose(key);
	delete key;
	delete name;
}

bool KeyRemoveCommand::execute( )
{
	char *namee = new char[keyGetNameSize(key)];
	keyGetName(key, namee, keyGetNameSize(key));
	
	if (kdbRemove(namee))
	{
		mainWidget()->showInStatusBar(strerror(errno));
		
		return false;
	}
	
	delete namee;
	delete mainWidget()->keyTree->currentItem();
	return true;
}

bool KeyRemoveCommand::unexecute( )
{
	if (kdbSetKey(key))
	{
		cout << strerror(errno) << endl;
		mainWidget()->showInStatusBar(strerror(errno));
		return false;
	}
	cout << "undeleting " << name << endl;
	//cout << "father is " << father->text(0) << endl;
	QListViewItem *fatherItem = mainWidget()->keyTree->findItem(father, 0, Qt::ExactMatch);
	QListViewItem *item = new QListViewItem(fatherItem, name);
	item->setPixmap(0, icon);
	return true;
}



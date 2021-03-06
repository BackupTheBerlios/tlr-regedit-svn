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

#include "mainwindowimpl.h"
#include "mainwidgetimpl.h"
#include "regedit_globals.h"

extern "C"
{
	#include <kdb.h>
}
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

#include <qmainwindow.h>
#include <qtoolbar.h>
#include <qpushbutton.h>
#include <qevent.h>
#include <qaction.h>
#include <qkeysequence.h>
#include <qstatusbar.h>

MainWindowImpl::MainWindowImpl (QWidget *parent, const char *name, WFlags fl)
: QMainWindow(parent, name, fl)
{
	editToolBar = new QToolBar(this, "The Main Windows ToolBar");
	editToolBar->setLabel("edit Commands");
	
	makeActions();
	mainWidget = new MainWidgetImpl(this, "The Main Widget");
	setCentralWidget(mainWidget);
	centralWidget()->show();
	setUpGui();
	
	connect(reload, SIGNAL(activated()), mainWidget, SLOT(updateKeyTree()));
	connect(undo, SIGNAL(activated()), mainWidget, SLOT(undo()));
	connect(redo, SIGNAL(activated()), mainWidget, SLOT(redo()));
	connect(del, SIGNAL(activated()), mainWidget, SLOT(deleteKey()));
	connect(newkey, SIGNAL(activated()), mainWidget, SLOT(addNewKey()));
	
	show();
}



/**
 * 
 */
void MainWindowImpl::setUpGui()
{
	statusBar()->clear();
	
	kdbOpen();
	
	::Key width;
	::Key height;
	::Key x;
	::Key y;
	//::Key splitter;
	
	keyInit(&width);
	keyInit(&height);
	keyInit(&x);
	keyInit(&y);
	//keyInit(&splitter);
	
	keySetName(&width, strdup(keyPrefix + "/gui/width")); 
	keySetName(&height, strdup(keyPrefix + "/gui/height"));
	keySetName(&x, strdup(keyPrefix + "/gui/x"));
	keySetName(&y, strdup(keyPrefix + "/gui/y"));
	//keySetName(&splitter, strdup(keyPrefix + "/gui/splitter"));
	
	checkKeyMake(&width, KEY_TYPE_STRING);
	checkKeyMake(&height, KEY_TYPE_STRING);
	checkKeyMake(&x, KEY_TYPE_STRING);
	checkKeyMake(&y, KEY_TYPE_STRING);
	//checkKeyMake(&splitter, KEY_TYPE_STRING);
	
	int vwidth = this->width();
	int vheight = this->height();
	int vx = this->x();
	int vy = this->y();
	
	if (keyGetDataSize(&width))
	{
		char *w = new char[keyGetDataSize(&width)];
		keyGetString(&width, w, keyGetDataSize(&width));
		vwidth = atoi(w);
	}
	
	if (keyGetDataSize(&height))
	{
		char *w = new char[keyGetDataSize(&height)];
		keyGetString(&height, w, keyGetDataSize(&height));
		vheight = atoi(w);
	}
	
	if (keyGetDataSize(&x))
	{
		char *w = new char[keyGetDataSize(&x)];
		keyGetString(&x, w, keyGetDataSize(&x));
		vx = atoi(w);
	}
	
	if (keyGetDataSize(&y))
	{
		char *w = new char[keyGetDataSize(&y)];
		keyGetString(&y, w, keyGetDataSize(&y));
		vy = atoi(w);
	}
	
	move(vx, vy);
	resize(vwidth, vheight);
	
	/*if (keyGetDataSize(&splitter))
	{
		char *w = new char[keyGetDataSize(&splitter)];
		splitter->moveSplitter(atoi(keyGetString(&splitter, w, keyGetSize(&splitter), 
	}*/
	//kdbClose();
}

void MainWindowImpl::closeEvent(QCloseEvent *e)
{
	//kdbOpen();
	
	::Key width;
	::Key height;
	::Key x;
	::Key y;
	QString guiKeyPrefix =  keyPrefix+"/gui/";
	
	keyInit(&width);
	keyInit(&height);
	keyInit(&x);
	keyInit(&y);
	
	
	keySetName(&width, strdup(guiKeyPrefix + "width"));
	keySetName(&height, strdup(guiKeyPrefix + "height"));
	keySetName(&x, strdup(guiKeyPrefix + "x"));
	keySetName(&y, strdup(guiKeyPrefix + "y"));
	
	kdbGetKey(&width);
	keySetString(&width, strdup(QString().setNum(this->width())));
	keySetComment(&width, "This is where regedit stores the width of the window");
	kdbSetKey(&width);
	keyClose(&width);
	
	kdbGetKey(&height);
	keySetString(&height, strdup(QString().setNum(this->height())));
	keySetComment(&height, "This is where regedit stores the height of the window");
	kdbSetKey(&height);
	keyClose(&height);
	
	kdbGetKey(&x);
	keySetString(&x, strdup(QString().setNum(this->x())));
	keySetComment(&x, "This is where regedit stores the x position of the window");
	kdbSetKey(&x);
	keyClose(&x);
	
	kdbGetKey(&y);
	keySetString(&y, strdup(QString().setNum(this->y())));
	keySetComment(&y, "This is where regedit stores the y position of the window");
	kdbSetKey(&y);
	keyClose(&y);
	
	kdbClose();
	
	QMainWindow::closeEvent(e);
}

void MainWindowImpl::updateActions( )
{
	//cout << "updating actions" << endl;
	char * keyname = mainWidget->getSelected();
	::Key *selected = new ::Key;
	keyInit(selected);
	keySetName(selected, keyname);
	kdbGetKey(selected);
	
	if (!selected)
	{
		cout << "can not update actions on null key" << endl;
		return;
	}
	
	
	if (mainWidget->canRedo())
		redo->setEnabled(true);
	else
		redo->setEnabled(false);
		
	if (mainWidget->canUndo())
		undo->setEnabled(true);
	else
		undo->setEnabled(false);
	
	
	
	if (keyGetType(selected) == KEY_TYPE_DIR)
	{
	
		newkey->setEnabled(true);
		
		KeySet childs;
		ksInit(&childs);
		
		char *keyname = new char[keyGetNameSize(selected)];
		keyGetName(selected, keyname, keyGetNameSize(selected));
		
		cout << keyname << endl;
		
		if (kdbGetChildKeys(keyname, &childs, KDB_O_DIR|KDB_O_SORT))
		{
			statusBar()->message(strerror(errno));
		}
		else
		{
			if (ksGetSize(&childs))
				del->setEnabled(false);
			else
				del->setEnabled(true);
		}
		delete keyname;
	}
	else
	{
		newkey->setEnabled(false);
		if (keyGetUID(selected) == getuid())
			del->setEnabled(true);
		else
			del->setEnabled(false);
	}
	
}

QAction * MainWindowImpl::getUndoAction( )
{
	return undo;
}

QAction * MainWindowImpl::getRedoAction( )
{
	return redo;
}

QAction * MainWindowImpl::getReloadAction( )
{
	return reload;
}

QAction * MainWindowImpl::getDeleteAction( )
{
	return del;
}

QAction * MainWindowImpl::getNewKeyAction( )
{
	return newkey;
}

void MainWindowImpl::makeActions( )
{
	QString iconDir(getIconDir());
	
	QPixmap newIcon(iconDir + "/filenew.png");
	QPixmap delIcon(iconDir + "/editdelete.png");
	QPixmap reloadIcon(iconDir + "/reload.png");
	QPixmap undoIcon(iconDir + "/undo.png");
	QPixmap redoIcon(iconDir + "/redo.png");
	
	newkey = new QAction(QString("add new key to kdb"), QIconSet(newIcon), QString("new key"), QKeySequence(CTRL + Key_N), this, "newkey action");
	del = new QAction(QString("delete key from kdb"), QIconSet(delIcon), QString("delet key"), QKeySequence(Key_Delete), this, "delete action");
	reload = new QAction(QString("load the kdb"), QIconSet(reloadIcon), QString("refresh"), QKeySequence(Key_F5), this, "refresh action");
	undo = new QAction(QString("undo last modification"), QIconSet(undoIcon), QString("undo"), QKeySequence(CTRL + Key_Z), this, "undo action");
	redo = new QAction(QString("redo last modification"), QIconSet(redoIcon), QString("redo"), QKeySequence(CTRL + Key_R), this, "redo action");
	
	newkey->addTo(editToolBar);
	del->addTo(editToolBar);
	editToolBar->addSeparator();
	reload->addTo(editToolBar);	
	undo->addTo(editToolBar);
	redo->addTo(editToolBar);
	
	newkey->setEnabled(false);
	del->setEnabled(false);
	undo->setEnabled(false);
	redo->setEnabled(false);
	
	
}

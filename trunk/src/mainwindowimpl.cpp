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
	#include <registry.h>
}
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
	mainWidget = new MainWidgetImpl(this, "The Main Widget");
	setCentralWidget(mainWidget);
	centralWidget()->show();
	setUpGui();
	
	show();
}

/**
 * 
 */
void MainWindowImpl::setUpGui()
{
	statusBar()->clear();
	editToolBar = new QToolBar(this, "The Main Windows ToolBar");
	editToolBar->setLabel("edit Commands");
	
	QString iconDir(getIconDir());
	
	QPixmap newIcon(iconDir + "/filenew.png");
	QPixmap delIcon(iconDir + "/editdelete.png");
	QPixmap reloadIcon(iconDir + "/reload.png");
	QPixmap undoIcon(iconDir + "/undo.png");
	QPixmap redoIcon(iconDir + "/redo.png");
	
	newkey = new QAction(QIconSet(newIcon), QString("new key"), QKeySequence(CTRL + Key_N), this, "newkey action");
	del = new QAction(QIconSet(delIcon), QString("delet key"), QKeySequence(Key_Delete), this, "delete action");
	reload = new QAction(QIconSet(reloadIcon), QString("refresh"), QKeySequence(Key_F5), this, "refresh action");
	undo = new QAction(QIconSet(undoIcon), QString("undo"), QKeySequence(CTRL + Key_Z), this, "undo action");
	redo = new QAction(QIconSet(redoIcon), QString("redo"), QKeySequence(CTRL + Key_R), this, "redo action");
	
	newkey->addTo(editToolBar);
	del->addTo(editToolBar);
	editToolBar->addSeparator();
	reload->addTo(editToolBar);	
	undo->addTo(editToolBar);
	redo->addTo(editToolBar);
	
	newkey->setDisabled(true);
	del->setEnabled(false);
	undo->setEnabled(false);
	redo->setEnabled(false);
	
	connect(reload, SIGNAL(activated()), mainWidget, SLOT(updateKeyTree()));
	connect(undo, SIGNAL(activated()), mainWidget, SLOT(undo()));
	connect(redo, SIGNAL(activated()), mainWidget, SLOT(redo()));
	
	registryOpen();
	
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
	
	checkKeyMake(&width, RG_KEY_TYPE_STRING);
	checkKeyMake(&height, RG_KEY_TYPE_STRING);
	checkKeyMake(&x, RG_KEY_TYPE_STRING);
	checkKeyMake(&y, RG_KEY_TYPE_STRING);
	//checkKeyMake(&splitter, RG_KEY_TYPE_STRING);
	
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
	registryClose();
}

void MainWindowImpl::closeEvent(QCloseEvent *e)
{
	registryOpen();
	
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
	
	registryGetKey(&width);
	keySetString(&width, strdup(QString().setNum(this->width())));
	keySetComment(&width, "This is where regedit stores the width of the window");
	registrySetKey(&width);
	keyClose(&width);
	
	registryGetKey(&height);
	keySetString(&height, strdup(QString().setNum(this->height())));
	keySetComment(&height, "This is where regedit stores the height of the window");
	registrySetKey(&height);
	keyClose(&height);
	
	registryGetKey(&x);
	keySetString(&x, strdup(QString().setNum(this->x())));
	keySetComment(&x, "This is where regedit stores the x position of the window");
	registrySetKey(&x);
	keyClose(&x);
	
	registryGetKey(&y);
	keySetString(&y, strdup(QString().setNum(this->y())));
	keySetComment(&y, "This is where regedit stores the y position of the window");
	registrySetKey(&y);
	keyClose(&y);
	
	registryClose();
	
	QMainWindow::closeEvent(e);
}

void MainWindowImpl::updateActions( )
{
	cout << "updating actions" << endl;
	if (mainWidget->canRedo())
		redo->setEnabled(true);
	else
		redo->setEnabled(false);
		
	if (mainWidget->canUndo())
		undo->setEnabled(true);
	else
		undo->setEnabled(false);
	
	::Key *selected = mainWidget->getSelected();
	
	if (keyGetType(selected) == RG_KEY_TYPE_DIR)
	{
		newkey->setEnabled(true);
		
		KeySet childs;
		ksInit(&childs);
		registryGetChildKeys(selected->key, &childs, RG_O_DIR|RG_O_SORT);
		
		if (childs.size)
			del->setEnabled(false);
		else
			del->setEnabled(true);
	}
	else
	{
		newkey->setEnabled(false);
		del->setEnabled(true);
	}
	
}

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

MainWindowImpl::MainWindowImpl (QWidget *parent, const char *name, WFlags fl)
: QMainWindow(parent, name, fl)
{
	mainWidget = new MainWidgetImpl(this, "The Main Widget");
	setCentralWidget(mainWidget);
	centralWidget()->show();
	show();
	setUpGui();
}

void MainWindowImpl::setUpGui()
{
	//editToolBar = new QToolBar(this, "The Main Windows ToolBar");
	registryOpen();
	::Key width;
	::Key height;
	::Key x;
	::Key y;
	::Key splitter;
	
	keyInit(&width);
	keyInit(&height);
	keyInit(&x);
	keyInit(&y);
	keyInit(&splitter);
	
	keySetName(&width, strdup(keyPrefix + "/gui/width")); 
	keySetName(&height, strdup(keyPrefix + "/gui/height"));
	keySetName(&x, strdup(keyPrefix + "/gui/x"));
	keySetName(&y, strdup(keyPrefix + "/gui/y"));
	keySetName(&splitter, strdup(keyPrefix + "/gui/splitter"));
	
	checkKeyMake(&width, RG_KEY_TYPE_STRING);
	checkKeyMake(&height, RG_KEY_TYPE_STRING);
	checkKeyMake(&x, RG_KEY_TYPE_STRING);
	checkKeyMake(&y, RG_KEY_TYPE_STRING);
	checkKeyMake(&splitter, RG_KEY_TYPE_STRING);
	
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
	cout << "preparing for closing" << endl;
	registryOpen();
	
	::Key width;
	::Key height;
	::Key x;
	::Key y;
	QString guiKeyPrefix =  keyPrefix+"/gui/";
	cout << guiKeyPrefix << endl;
	
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
	registrySetKey(&width);
	keyClose(&width);
	
	registryGetKey(&height);
	keySetString(&height, strdup(QString().setNum(this->height())));
	registrySetKey(&height);
	keyClose(&height);
	
	registryGetKey(&x);
	keySetString(&x, strdup(QString().setNum(this->x())));
	registrySetKey(&x);
	keyClose(&x);
	
	registryGetKey(&y);
	keySetString(&y, strdup(QString().setNum(this->y())));
	registrySetKey(&y);
	keyClose(&y);
	
	registryClose();
	
	QMainWindow::closeEvent(e);
}

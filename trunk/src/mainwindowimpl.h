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

#ifndef MAINWINDOWIMPL_H
#define MAINWINDOWIMPL_H
 
#include <qmainwindow.h>

class QWidget;
class MainWidgetImpl;
class QToolBar;
class QCloseEvent;
 
class MainWindowImpl : public QMainWindow
{
Q_OBJECT

public:
	MainWindowImpl(QWidget *parent = 0, const char *name = 0, WFlags f = WType_TopLevel);
	
private:
	void setUpGui();
	void closeEvent(QCloseEvent *e);
	MainWidgetImpl *mainWidget;
	QToolBar *editToolBar;
	QToolBar *othersToolBar;
};
 
#endif

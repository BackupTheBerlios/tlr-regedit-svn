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

#ifndef MAINWIDGETIMPL_H
#define MAINWIDGETIMPL_H


extern "C"
{
	#include <kdb.h>
}
#include <stack>

#include "regedit_globals.h"
#include "mainwidget.h"
#include "command.h"
#include <qpoint.h>
#include <qpixmap.h>
#include <qpopupmenu.h>
#include <qptrstack.h>

#define COMBO_POS_UND 0
#define COMBO_POS_LNK 1
#define COMBO_POS_STR 2
#define COMBO_POS_BIN 3
#define COMBO_POS_DIR 4

 

class QCloseEvent;
class QListViewItem;
class MainWindowImpl;

class MainWidgetImpl : public MainWidget
{
Q_OBJECT

public:
	MainWidgetImpl(QWidget *parent = 0, const char *name = 0, WFlags fl = 0);
	~MainWidgetImpl();
	bool canUndo();
	bool canRedo();
	char *getSelected();
	mode_t getSelectedAccess();
	void showInStatusBar(QString str);
	QPixmap dirIcon;
	QPixmap stringIcon;
	QPixmap binaryIcon;
	QPixmap linkOverlay;
	QPixmap lockOverlay;
	QPixmap deniedIcon;
	
	int *types;
	
signals:
	void keyChanged();
	
public slots:
	void changeSelected(QListViewItem *item);
	void showKeyValues(bool update = false);
	void updateKeyTree();
	void undo();
	void redo();
	void addNewKey();
	void deleteKey();
	
private:
	void fillUpKeyTree(::Key *root, QListViewItem *item);
	void setUpGui();
	void setWidgetsEnabled(bool enabled);
	void pushUndo(Command *cmd);
	
	QPopupMenu keyPopupMenu;
	bool ignoreTextChanges;
	char *selected;
	mode_t selectedAccess;	
	MainWindowImpl *parent;
	
	QPtrStack<Command> undoStack;
	QPtrStack<Command> redoStack;
	
	
private slots:

	
	QString getKeyNameFromItem(QListViewItem *item);
	void showItemMenu(QListViewItem * item, const QPoint &p, int b);
	void keyAttributesChanged(const QString &s); 
	void commentAttributeChanged();
	void keyTypeChanged(int id);
	void changeAccessMode();
	void copyNameToClipboard();
	void copyValueToClipboard();
	void revokeChanges();
	void applyChanges();
	void clearRedoStack();
};

#endif

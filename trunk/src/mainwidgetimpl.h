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
	#include <registry.h>
}

#include "regedit_globals.h"
#include "mainwidget.h"
#include <qpoint.h>
#include <qpixmap.h>

#define COMBO_POS_UND 0
#define COMBO_POS_LNK 1
#define COMBO_POS_STR 2
#define COMBO_POS_BIN 3
#define COMBO_POS_DIR 4

class QCloseEvent;
class QListViewItem;

class MainWidgetImpl : public MainWidget
{
Q_OBJECT

public:
	MainWidgetImpl(QWidget *parent = 0, const char *name = 0, WFlags fl = 0);
	~MainWidgetImpl();
	void updateKeyTree();
	
private:
	
	void fillUpKeyTree(::Key *root, QListViewItem *item);
	void setUpGui();
	bool ignoreTextChanges;
	
	QPixmap stringIcon;
	QPixmap binaryIcon;
	QPixmap linkIcon;
	QPixmap dirIcon;
	
private slots:
	void showKeyValues(QListViewItem *selected);
	QString getKeyNameFromItem(QListViewItem *item);
	void showItemMenu(QListViewItem *item, const QPoint &p, int b);
	void keyAttributesChanged(const QString &s);
	void commentAttributeChanged();
	void keyTypeChanged(int id);
	void addNewKey();
	void addNewDir();
	void copyNameToClipboard();
	void copyValueToClipboard();
	void deleteKey();
	void revokeChanges();
	void applyChanges();
};

#endif

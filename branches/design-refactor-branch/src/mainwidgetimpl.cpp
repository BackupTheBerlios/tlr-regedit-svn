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

#include "mainwidgetimpl.h"
#include "mainwindowimpl.h"
#include "newkeydialogimpl.h"
#include "permissiondialogimpl.h"
#include "regedit_globals.h"

#include "keyremovecommand.h"
#include "keymodifycommand.h"
#include "keyaddcommand.h"

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <iostream>
#include <errno.h>
#include <unistd.h>

using namespace std;

#include <qmainwindow.h>
#include <qlistview.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qdatetime.h>
#include <qpopupmenu.h>
#include <qpushbutton.h>
#include <qinputdialog.h>
#include <qtextedit.h>
#include <qcombobox.h>
#include <qclipboard.h>
#include <qapplication.h>
#include <qaction.h>
#include <qstatusbar.h>
#include <qcheckbox.h>

/**
 *the main constructor
 *it builds up the treeview representing the regestry structure
 *
 */

MainWidgetImpl::MainWidgetImpl(QWidget *parent, const char *name, WFlags fl) 
	: MainWidget(parent, name, fl), ignoreTextChanges(false), selected(0), selectedAccess(0), parent( (MainWindowImpl *) parentWidget())
{
	types = new int[5];
	types[0] = KEY_TYPE_UNDEFINED;
	types[1] = KEY_TYPE_LINK;
	types[2] = KEY_TYPE_STRING;
	types[3] = KEY_TYPE_BINARY;
	types[4] = KEY_TYPE_DIR;
	int *t;
	t = new int[4];
	ignoreTextChanges = false;		//a flag to indicate wheater the program or the user has edited the fields
	
	undoStack.setAutoDelete(false);
	redoStack.setAutoDelete(false);
	
	connect(keyTree, SIGNAL(selectionChanged(QListViewItem*)), this, SLOT(changeSelected(QListViewItem*)));
	connect(keyTree, SIGNAL(rightButtonClicked(QListViewItem*, const QPoint&, int)), this, SLOT(showItemMenu(QListViewItem*, const QPoint &, int)));
	
	connect(keyName, SIGNAL(textChanged(const QString &)), this, SLOT(keyAttributesChanged(const QString &)));
	connect(keyValue, SIGNAL(textChanged(const QString &)), this, SLOT(keyAttributesChanged(const QString &)));
	connect(keyComment, SIGNAL(textChanged()), this, SLOT(commentAttributeChanged()));
	connect(typeCombo, SIGNAL(activated (int)), this, SLOT(keyTypeChanged(int)));
	
	connect(revokeButton, SIGNAL(clicked()), this, SLOT(revokeChanges()));
	connect(applyButton, SIGNAL(clicked()), this, SLOT(applyChanges()));
	
	connect(editButton, SIGNAL(clicked()), this, SLOT(changeAccessMode()));
	
	connect(this, SIGNAL(keyChanged()), parent, SLOT(updateActions()));
	connect(this, SIGNAL(keyChanged()), this, SLOT(showKeyValues()));
	
	setWidgetsEnabled(false);
	setUpGui();
	updateKeyTree();
}



void MainWidgetImpl::setUpGui()
{	
	keyTree->setTreeStepSize(12);
	
	if (getIconDir() != QString::null)
	{
		QString prefix(getIconDir());
		binaryIcon = QPixmap(prefix + "/binary.png");
		dirIcon = QPixmap(prefix + "/folder.png");
		stringIcon = QPixmap(prefix + "/txt.png");
		linkOverlay = QPixmap(prefix +"/link.png");
		lockOverlay = QPixmap(prefix + "/lockoverlay.png");
		deniedIcon = QPixmap(prefix + "/lock.png");
		if (binaryIcon.isNull())
			cout << "icon is null" << endl;
	}
	parent->getNewKeyAction()->addTo(&keyPopupMenu);
	parent->getDeleteAction()->addTo(&keyPopupMenu);
	keyPopupMenu.insertSeparator();
}

/*QListView *MainWidgetImpl::getKeyTree()
{
	return keyTree;
}*/

void MainWidgetImpl::updateKeyTree()
{
	keyTree->clear();
	//kdbOpen();
	
	KeySet roots;	
	ksInit(&roots);
	
	kdbGetRootKeys(&roots);
	::Key *mover;
	
	mover = ksNext(&roots);
	
	while (mover)
	{
		QListViewItem *item = new QListViewItem(keyTree, mover->key);
		
		switch (keyGetType(mover))
		{
			case KEY_TYPE_DIR:
				item->setPixmap(0, dirIcon);
				fillUpKeyTree(mover, item);
				break;
			case KEY_TYPE_LINK:
				item->setPixmap(0, linkOverlay);
				break;
			case KEY_TYPE_STRING:
				item->setPixmap(0, stringIcon);
				break;
			case KEY_TYPE_BINARY:
				item->setPixmap(0, binaryIcon);
		}
		mover = ksNext(&roots);
	}
	//keyClose(mover);
	ksClose(&roots);
	//kdbClose();
}

/**
 * fills up the treeview recursively
 * only called once by the constructor
 * can later be called by other functions to implement a refresh functionality
 */
void MainWidgetImpl::fillUpKeyTree(::Key *root, QListViewItem *item)
{
	KeySet keys;
	
	ksInit(&keys);
	char *keyname = new char[keyGetNameSize(root)];
	keyGetName(root, keyname, keyGetNameSize(root));
	if (keyGetType(root) != KEY_TYPE_DIR)
		return;
	kdbGetChildKeys(keyname, &keys, KDB_O_DIR|KDB_O_SORT);
	
	delete keyname;
	::Key *mover = ksNext(&keys);;
	
	while (mover)
	{
		
		QListViewItem *subItem = new QListViewItem(item, QString(mover->key).section(RG_KEY_DELIM, -1));
		
		int type = keyGetType(mover);
			
		if (type)
		{
			switch (type)
			{
				case KEY_TYPE_DIR:
					subItem->setPixmap(0, dirIcon);
					fillUpKeyTree(mover, subItem);
					break;
				case KEY_TYPE_LINK:
					subItem->setPixmap(0, linkOverlay);
					break;
				case KEY_TYPE_STRING:
					subItem->setPixmap(0, stringIcon);
					break;
				case KEY_TYPE_BINARY:
					subItem->setPixmap(0, binaryIcon);
					break;
				default:
					cout << "should not happen: type Is:" << type << endl;
					cout << mover->key << endl;
			}
		}
		else
		{
			subItem->setPixmap(0, deniedIcon);
			/*cout << mover->key << endl;
			perror("fillUpKeyTree");*/
		}
		mover = ksNext(&keys);
	}
	ksClose(&keys);
}

MainWidgetImpl::~MainWidgetImpl()
{
	
}
 

void MainWidgetImpl::showKeyValues(bool update)
{
	ignoreTextChanges = true;
	
	revokeButton->setEnabled(false);
	applyButton->setEnabled(false);
	
	setWidgetsEnabled(true);
		
	keyName->clear();
	keyValue->clear();
	keyComment->clear();
	
	userID->clear();
	groupID->clear();
	permission->clear();
	
	keyATime->clear();
	keyMTime->clear();
	keyCTime->clear();
	
	
	
	if (!update)
		showInStatusBar(QString(""));
	
	char *buffer;
	
	char *keyname = getSelected();
	::Key *selectedKey = new ::Key;
	keySetName(selectedKey, keyname);
	kdbGetKey(selectedKey);
	
	if (keyGetNameSize(selectedKey))
	{
		keyName->setText(QString(keyname).section(RG_KEY_DELIM, -1));
	}
	
	if (selected == 0)
	{
		showInStatusBar(strerror(errno));
		setWidgetsEnabled(false);
		return;
	}
	
	if (keyGetUID(selectedKey) != getuid())
		editButton->setEnabled(false);
	
	if (typeCombo->count() - COMBO_POS_DIR)
		typeCombo->removeItem(COMBO_POS_DIR);
	
	switch (keyGetType(selectedKey))
	{
		case KEY_TYPE_BINARY: 
			typeCombo->setCurrentItem(COMBO_POS_BIN);
			keyValue->setEnabled(true);
			keyComment->setEnabled(true);
			typeCombo->setEnabled(true);
			break;
		case KEY_TYPE_STRING:
			typeCombo->setCurrentItem(COMBO_POS_STR);
			keyValue->setEnabled(true);
			keyComment->setEnabled(true);
			typeCombo->setEnabled(true);
			break;
		case KEY_TYPE_DIR:
			typeCombo->insertItem("Directory");
			typeCombo->setCurrentItem(COMBO_POS_DIR);
			typeCombo->setEnabled(false);
			keyValue->setEnabled(false);
			keyComment->setEnabled(false);
			break;
		case KEY_TYPE_LINK:
			typeCombo->setCurrentItem(COMBO_POS_LNK);
			break;
		case KEY_TYPE_UNDEFINED:
			typeCombo->setCurrentItem(COMBO_POS_UND);
			keyValue->setEnabled(false);
			keyName->setEnabled(false);
			keyComment->setEnabled(false);
			break;
		default:
			buffer = "";
			cout << "none of the types set" << endl;
	}
	
	if (keyGetDataSize(selectedKey))
	{
		buffer = new char[keyGetDataSize(selectedKey)];
		int size = keyGetDataSize(selectedKey);
		switch (keyGetType(selectedKey))
		{
			case KEY_TYPE_BINARY: 
				keyGetBinary(selectedKey, buffer, size);
				break;
			case KEY_TYPE_STRING:
				keyGetString(selectedKey, buffer, size);
				break;
			case KEY_TYPE_DIR:
				break;
			case KEY_TYPE_LINK:
				break;
			case KEY_TYPE_UNDEFINED:
				break;
			default:
				buffer = "";
				cout << "none of the types set" << endl;
		}
		keyValue->setText(buffer);
		delete buffer;
	}

	if (keyGetCommentSize(selectedKey))
	{
		buffer = new char[keyGetCommentSize(selectedKey)];
		keyGetComment(selectedKey, buffer, keyGetCommentSize(selectedKey));
		keyComment->setText(buffer);
		delete buffer;
	}
	
	QDateTime dt;
	
	dt.setTime_t(keyGetATime(selectedKey));
	keyATime->setText(dt.toString());

	dt.setTime_t(keyGetMTime(selectedKey));
	keyMTime->setText(dt.toString());
	
	dt.setTime_t(keyGetCTime(selectedKey));
	keyCTime->setText(dt.toString());
	
	struct passwd *pwd = getpwuid(keyGetUID(selectedKey));
	struct group *grp = getgrgid(keyGetGID(selectedKey));
	
	userID->setText(pwd->pw_name);
	
	groupID->setText(grp->gr_name);
	
	mode_t mode = selectedAccess;
	
	char *readable = new char[10];
	
	if (S_ISDIR(mode)) 
		readable[0]='d';
	else 
		if (S_ISLNK(mode)) readable[0]='l';
			else readable[0]='-';
	readable[1] = mode & S_IRUSR ? 'r' : '-';
	readable[2] = mode & S_IWUSR ? 'w' : '-';
	readable[3] = mode & S_IXUSR ? 'x' : '-';
	readable[4] = mode & S_IRGRP ? 'r' : '-';
	readable[5] = mode & S_IWGRP ? 'w' : '-';
	readable[6] = mode & S_IXGRP ? 'x' : '-';
	readable[7] = mode & S_IROTH ? 'r' : '-';
	readable[8] = mode & S_IWOTH ? 'w' : '-';
	readable[9] = mode & S_IXOTH ? 'x' : '-';
	readable[10]= 0;
	
	permission->setText(readable);
	delete readable;
	
	ignoreTextChanges = false;

}

void MainWidgetImpl::setWidgetsEnabled(bool enabled)
{
	keyName->setEnabled(enabled);
	keyValue->setEnabled(enabled);
	keyComment->setEnabled(enabled);
	typeCombo->setEnabled(enabled);
	keyATime->setEnabled(enabled);
	keyMTime->setEnabled(enabled);
	keyCTime->setEnabled(enabled);
	
	userID->setEnabled(enabled);
	groupID->setEnabled(enabled);
	permission->setEnabled(enabled);
	editButton->setEnabled(enabled);
	
}
 
void MainWidgetImpl::changeSelected(QListViewItem *item)
{	
	delete selected;
	
	if (!item)
	{
		cout << " item null " << endl;
		selected = 0;
		selectedAccess = 0;
		emit keyChanged();
		return;
	}
	
	if (!item->isVisible())
	{
		cout << " item not visible " << endl;
		selected = 0;
		selectedAccess = 0;
		emit keyChanged();
		return;
 	}
	
	selected = strdup(getKeyNameFromItem(item));
	
	::Key *selectedKey = new ::Key;
	keyInit(selectedKey);
	keySetName(selectedKey, selected);

	if (kdbGetKey(selectedKey))
	{
		showInStatusBar(strerror(errno));
		delete selected;
		selected = 0;
		selectedAccess = 0;
		emit keyChanged();
		return;
	}
	
	
	selectedAccess = keyGetAccess(selectedKey);
	keyClose(selectedKey);
	delete selectedKey;
	emit keyChanged();
	
}

char * MainWidgetImpl::getSelected()
{
	return selected;
}

mode_t MainWidgetImpl::getSelectedAccess()
{
	return selectedAccess;
}

/**
 * returns the full keyname from an item in the treeview
 */
QString MainWidgetImpl::getKeyNameFromItem(QListViewItem *item)
{
	QString key = item->text(0);
	QListViewItem *parent = item->parent();
	
	while (parent != 0)
	{
		key = parent->text(0) + RG_KEY_DELIM + key;
		parent = parent->parent();
	}

	return key;
}

/**
 *pops up the kontextmenu 
 *gets called when the user rightclickes an item in the treeview (SIGNAL: rightButtonClicked(QListViewItem *item, QPos &pos, int b))
 */
void MainWidgetImpl::showItemMenu(QListViewItem *item, const QPoint &p, int b)
{
	keyPopupMenu.exec(p);
}

/**
 * undos changes made by the user 
 * gets called when the user hits the revokeButton 
 */
void MainWidgetImpl::revokeChanges()
{	
	revokeButton->setEnabled(false);
	applyButton->setEnabled(false);
}

/**
 * writes back the changes into the kdb made by the user
 * gets called when the user hits the apply button
 */
void MainWidgetImpl::applyChanges()
{
	if (!selected)
		return;
		
	KeyModifyCommand *cmd = new KeyModifyCommand(this, "Modify Key Command");
	
	if (cmd->execute())
	{
		pushUndo(cmd);
		clearRedoStack();
		
		applyButton->setEnabled(false);
		revokeButton->setEnabled(false);
		emit keyChanged();	
	}
}

void MainWidgetImpl::changeAccessMode()
{
	PermissionDialogImpl *perm = new PermissionDialogImpl(selectedAccess, this, "The Change Permission Dailog");
	
	if (perm->exec() == QDialog::Accepted)
	{
		if (perm->ur->isChecked()) selectedAccess |= S_IRUSR; else selectedAccess &= ~S_IRUSR;
		if (perm->uw->isChecked()) selectedAccess |= S_IWUSR; else selectedAccess &= ~S_IWUSR;
		if (perm->ux->isChecked()) selectedAccess |= S_IXUSR; else selectedAccess &= ~S_IXUSR;
		
		if (perm->gr->isChecked()) selectedAccess |= S_IRGRP; else selectedAccess &= ~S_IRGRP;
		if (perm->gw->isChecked()) selectedAccess |= S_IWGRP; else selectedAccess &= ~S_IWGRP;
		if (perm->gx->isChecked()) selectedAccess |= S_IXGRP; else selectedAccess &= ~S_IXGRP;
		
		if (perm->otr->isChecked()) selectedAccess |= S_IROTH; else selectedAccess &= ~S_IROTH;
		if (perm->ow->isChecked()) selectedAccess |= S_IWOTH; else selectedAccess &= ~S_IWOTH;
		if (perm->ox->isChecked()) selectedAccess |= S_IXOTH; else selectedAccess &= ~S_IXOTH;
		
		
		keyAttributesChanged("");
		emit keyChanged();
	}
}

void MainWidgetImpl::commentAttributeChanged()
{
	keyAttributesChanged(keyComment->text());
}

/**
 * 
 */
void MainWidgetImpl::keyAttributesChanged(const QString &p)
{
	p.length();	//suppress warnings
	if (ignoreTextChanges)
		return;
	applyButton->setEnabled(true);
	revokeButton->setEnabled(true);
}

void MainWidgetImpl::keyTypeChanged(int id)
{
	id = 0;		//suppress warnings
	keyAttributesChanged("");
}


/**
 * delete a key
 */
void MainWidgetImpl::deleteKey()
{
	if (!selected)
		return;
	
	KeyRemoveCommand *cmd = new KeyRemoveCommand(this, QString("remove command: ") + selected);
	
	cmd->execute();
	pushUndo(cmd);
	clearRedoStack();
	emit keyChanged();
}

void MainWidgetImpl::addNewKey()
{
	if (!selected)
		return;
		
	NewKeyDialogImpl *newDialog = new NewKeyDialogImpl(getKeyNameFromItem(keyTree->currentItem()), this);
	if (newDialog->exec())
	{
		KeyAddCommand *cmd = new KeyAddCommand(newDialog, this, QString("add command: ") + selected
		);
		cmd->execute();
		pushUndo(cmd);
		clearRedoStack();
		emit keyChanged();
	}
}

void MainWidgetImpl::copyNameToClipboard()
{
	QClipboard *cp = QApplication::clipboard();
	QString entry;
	entry = getKeyNameFromItem(keyTree->currentItem());
	
	cp->setText(entry, QClipboard::Clipboard);
}

void MainWidgetImpl::copyValueToClipboard()
{
	//kdbOpen();
	char *name = strdup(getKeyNameFromItem(keyTree->currentItem()));
	char *buf;
	::Key key;
	keySetName(&key, name);
	
	kdbGetKey(&key);
	
	buf = new char[keyGetDataSize(&key)];
	
	if (keyGetType(&key) == KEY_TYPE_STRING)
		keyGetString(&key, buf, keyGetDataSize(&key));
	else
		keyGetBinary(&key, (void *)buf, keyGetDataSize(&key));
	
	QClipboard *cp = QApplication::clipboard();
	cp->setText(buf, QClipboard::Clipboard);
	keyClose(&key);
	//kdbClose();
}

bool MainWidgetImpl::canUndo()
{
	return undoStack.count();
}

bool MainWidgetImpl::canRedo()
{
	return redoStack.count();
}

void MainWidgetImpl::pushUndo(Command *cmd)
{
	undoStack.push(cmd);
	clearRedoStack();
	emit keyChanged();
}

void MainWidgetImpl::undo()
{
	if (undoStack.isEmpty())
	{
		cout << "programm error please report" << endl;
		return;
	}
	
	Command *cmd = undoStack.pop();
	//if (undoStack.remove())	cout << "removed first element" << endl;
	cmd->unexecute();
	redoStack.push(cmd);
	changeSelected(keyTree->currentItem());
	//showKeyValues();
	//emit keyChanged();
}

void MainWidgetImpl::redo() 
{
	if (redoStack.isEmpty())
	{
		cout << "programm error please report" << endl;
		return;
	}
	Command *cmd = redoStack.pop();
	//redoStack.remove();
	cmd->execute();
	undoStack.push(cmd);
	changeSelected(keyTree->currentItem());
	//showKeyValues();
	//emit keyChanged();
}

void MainWidgetImpl::clearRedoStack()
{
	redoStack.clear();
}

void MainWidgetImpl::showInStatusBar( QString str )
{
	parent->statusBar()->message(str);
}

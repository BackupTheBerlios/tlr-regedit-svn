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
#include "newkeydialogimpl.h"

#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <iostream>
#include <errno.h>
#include <unistd.h>

using namespace std;

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

/**
 *the main constructor
 *it builds up the treeview representing the regestry structure
 *
 */

MainWidgetImpl::MainWidgetImpl(QWidget *parent, const char *name, WFlags fl) 
	: MainWidget(parent, name, fl), dirIcon(QPixmap("icons/folder.png")), stringIcon(QPixmap("icons/txt.png")), 
	  binaryIcon(QPixmap("icons/binary.png")), keyPrefix("user/sw/regedit")
{
	registryOpen();
	
	setUpGui();
	
	::Key iconDir;
	
	keyInit(&iconDir);
	
	keySetName(&iconDir, "system/sw/regedit/gui/iconDir");
	
	int ret = registryGetKey(&iconDir);
	
	if (ret)
	{
		cout << "using user namespace iconDir" << endl;
		keyClose(&iconDir);
		keyInit(&iconDir);
		keySetName(&iconDir, "user/sw/regedit/gui/iconDir");
		checkKeyMake(&iconDir, RG_KEY_TYPE_STRING);
	}
	else
		cout << "using system namespace iconDir" << endl;
	
	if (keyGetDataSize(&iconDir))
	{
		char *buf = new char[keyGetDataSize(&iconDir)];
		
		keyGetString(&iconDir, buf, keyGetDataSize(&iconDir));
		QString prefix(buf);
		binaryIcon = QPixmap(prefix + "/binary.png");
		dirIcon = QPixmap(prefix + "/folder.png");
		stringIcon = QPixmap(prefix + "/txt.png");
		linkIcon = QPixmap(prefix +"/link.pnt");
	}
	
	keyClose(&iconDir);
	
	KeySet roots;	
	ksInit(&roots);
	
	registryGetRootKeys(&roots);
	
	::Key *mover;
	
	//keyInit(mover);
	
	mover = roots.start;
	
	while (mover != 0)
	{
		QListViewItem *item = new QListViewItem(keyTree, mover->key);
		fillUpKeyTree(mover, item);
		switch (keyGetType(mover))
		{
			case RG_KEY_TYPE_DIR:
				item->setPixmap(0, dirIcon);
				break;
			case RG_KEY_TYPE_LINK:
				item->setPixmap(0, linkIcon);
				break;
			case RG_KEY_TYPE_STRING:
				item->setPixmap(0, stringIcon);
				break;
			case RG_KEY_TYPE_BINARY:
				item->setPixmap(0, binaryIcon);
		}
		mover = mover->next;
	}
	
	ksClose(&roots);
	registryClose();
	
	keyTree->setTreeStepSize(12);
	
	ignoreTextChanges = false;		//a flag to indicate wheater the program or the user has edited the fields
	
	connect(keyTree, SIGNAL(selectionChanged(QListViewItem*)), this, SLOT(showKeyValues(QListViewItem*)));
	connect(keyTree, SIGNAL(rightButtonClicked(QListViewItem*, const QPoint&, int)), this, SLOT(showItemMenu(QListViewItem*, const QPoint &, int)));
	
	connect(keyName, SIGNAL(textChanged(const QString &)), this, SLOT(keyAttributesChanged(const QString &)));
	connect(keyValue, SIGNAL(textChanged(const QString &)), this, SLOT(keyAttributesChanged(const QString &)));
	connect(keyComment, SIGNAL(textChanged()), this, SLOT(commentAttributeChanged()));
	connect(typeCombo, SIGNAL(activated (int)), this, SLOT(keyTypeChanged(int)));
	
	connect(revokeButton, SIGNAL(clicked()), this, SLOT(revokeChanges()));
	connect(applyButton, SIGNAL(clicked()), this, SLOT(applyChanges()));
}


void MainWidgetImpl::setUpGui()
{
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
}

MainWidgetImpl::~MainWidgetImpl()
{
	
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
	
	registryGetChildKeys(root->key, &keys, RG_O_DIR|RG_O_SORT);
	
	::Key *mover = keys.start;
	
	while (mover)
	{
		if (item == 0)
			cout << "i have no parent" << endl;	
		
		/*char *name = new char[keyGetNameSize(mover)];
		keyGetKeyName(mover, name, keyGetNameSize(mover));*/
		
		QListViewItem *subItem = new QListViewItem(item, QString(mover->key).section(RG_KEY_DELIM, -1));
		
		switch (keyGetType(mover))
		{
			case RG_KEY_TYPE_DIR:
				subItem->setPixmap(0, dirIcon);
				break;
			case RG_KEY_TYPE_LINK:
				subItem->setPixmap(0, linkIcon);
				break;
			case RG_KEY_TYPE_STRING:
				subItem->setPixmap(0, stringIcon);
				break;
			case RG_KEY_TYPE_BINARY:
				subItem->setPixmap(0, binaryIcon);
		}
		
		
		
		fillUpKeyTree(mover, subItem); 
		mover = mover->next;
	}
	ksClose(&keys);
}

/**
 * filles up the textfields with the values of the selected key
 * gets called when the user clickes a key in the treeview
 * TODO permission checking is implemented rudimentary
 */
void MainWidgetImpl::showKeyValues(QListViewItem *item)
{
	if (item == 0)
		return;
	
	ignoreTextChanges = true;
	
	revokeButton->setEnabled(false);
	applyButton->setEnabled(false);
		
	keyName->clear();
	keyValue->clear();
	keyComment->clear();
	
	userID->clear();
	groupID->clear();
	accessMode->clear();
	
	keyATime->clear();
	keyMTime->clear();
	keyCTime->clear();
	
	registryOpen();
	
	::Key key;
	
	keyInit(&key);	
	char *name = strdup(getKeyNameFromItem(item));
	
	keySetName(&key, name);

	int ret = registryGetKey(&key);
	
	if (ret)
	{		
		perror("open key");
		
		keyValue->setEnabled(false);
		keyName->setEnabled(false);
	}
	else
	{
		keyValue->setEnabled(true);
		keyName->setEnabled(true);
	}
	
	char *buffer;
	
	if (keyGetNameSize(&key))
	{
		buffer = (char *) malloc(keyGetNameSize(&key));
		keyGetName(&key, buffer, keyGetNameSize(&key));
		keyName->setText(QString(buffer).section(RG_KEY_DELIM, -1));
	}
	
	if (typeCombo->count() - COMBO_POS_DIR)
		typeCombo->removeItem(COMBO_POS_DIR);
	
	switch (keyGetType(&key))
	{
		case RG_KEY_TYPE_BINARY: 
			typeCombo->setCurrentItem(COMBO_POS_BIN);
			keyValue->setEnabled(true);
			keyComment->setEnabled(true);
			typeCombo->setEnabled(true);
			break;
		case RG_KEY_TYPE_STRING:
			typeCombo->setCurrentItem(COMBO_POS_STR);
			keyValue->setEnabled(true);
			keyComment->setEnabled(true);
			typeCombo->setEnabled(true);
			break;
		case RG_KEY_TYPE_DIR:
			typeCombo->insertItem("Directory");
			typeCombo->setCurrentItem(COMBO_POS_DIR);
			typeCombo->setEnabled(false);
			keyValue->setEnabled(false);
			keyComment->setEnabled(false);
			break;
		case RG_KEY_TYPE_LINK:
			typeCombo->setCurrentItem(COMBO_POS_LNK);
			break;
		case RG_KEY_TYPE_UNDEFINED:
			typeCombo->setCurrentItem(COMBO_POS_UND);
			keyValue->setEnabled(false);
			keyName->setEnabled(false);
			keyComment->setEnabled(false);
			break;
		default:
			buffer = "";
			cout << "none of the types set" << endl;
	}
	
	if (keyGetDataSize(&key))
	{
		buffer = (char *) malloc(keyGetDataSize(&key));
		int size = keyGetDataSize(&key);
		switch (keyGetType(&key))
		{
			case RG_KEY_TYPE_BINARY: 
				keyGetBinary(&key, buffer, size);
				
				break;
			case RG_KEY_TYPE_STRING:
				keyGetString(&key, buffer, size);
				
				break;
			case RG_KEY_TYPE_DIR:
				
				break;
			case RG_KEY_TYPE_LINK:
				
				break;
			case RG_KEY_TYPE_UNDEFINED:
				
				break;
			default:
				buffer = "";
				cout << "none of the types set" << endl;
		}
		keyValue->setText(buffer);
	}
	
	
	if (keyGetCommentSize(&key))
	{
		buffer = (char *) malloc(keyGetCommentSize(&key));
		keyGetComment(&key, buffer, keyGetCommentSize(&key));
		keyComment->setText(buffer);
	}
	
	
	QDateTime dt;
	
	dt.setTime_t(keyGetATime(&key));
	keyATime->setText(dt.toString());

	dt.setTime_t(keyGetMTime(&key));
	keyMTime->setText(dt.toString());
	
	dt.setTime_t(keyGetCTime(&key));
	keyCTime->setText(dt.toString());
	
	
	
	struct passwd *pwd = getpwuid(keyGetUID(&key));
	struct group *grp = getgrgid(keyGetGID(&key));
	
	userID->setText(pwd->pw_name);
	
	groupID->setText(grp->gr_name);
	
	accessMode->setText(QString().setNum(key.access));
	
	keyClose(&key);
	
	free(name);
	registryClose();
	
	ignoreTextChanges = false;
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
 *
 *TODO use icons 
 *TODO refine the menu
 */
void MainWidgetImpl::showItemMenu(QListViewItem *item, const QPoint &p, int b)
{
	if (item == 0)
		return;
	
	QPopupMenu *pop = new QPopupMenu(this, "key righclick popupmenu");
	
	::Key key;
	
	char *name = strdup(getKeyNameFromItem(item));
	
	keyInit(&key);
	keySetName(&key, name);
	
	registryGetKey(&key);
	
	if (keyGetType(&key) == RG_KEY_TYPE_DIR)
	{
		pop->insertItem("new Key", this, SLOT(addNewKey()), CTRL+Key_N);
		pop->insertItem("new Directory", this, SLOT(addNewDir()), CTRL+Key_D);
		
		KeySet ks;
		
		ksInit(&ks);
		
		registryGetChildKeys(name, &ks, RG_O_DIR);
		
		if (ks.size < 1)
			pop->insertItem("delete Directory", this, SLOT(deleteKey()), CTRL+Key_E);
			
		ksClose(&ks);
	}
	else
	{
		pop->insertItem("delete Key", this, SLOT(deleteKey()), CTRL+Key_E);
	}
	
	pop->insertSeparator();
	pop->insertItem("copy Name to Clipboard", this, SLOT(copyNameToClipboard()));
	
	if (keyGetType(&key) != RG_KEY_TYPE_DIR)
		pop->insertItem("copy Value to Clipboard", this, SLOT(copyValueToClipboard()));
	
	pop->exec(p);
	keyClose(&key);
}

/**
 * undos changes made by the user 
 * gets called when the user hits the revokeButton 
 * TODO find better name for revoke probably undo?
 */
void MainWidgetImpl::revokeChanges()
{	
	revokeButton->setEnabled(false);
	applyButton->setEnabled(false);
}

/**
 * writes back the changes into the registry made by the user
 * gets called when the user hits the apply button
 */
void MainWidgetImpl::applyChanges()
{
	registryOpen();
	::Key oldKey;
	QString qsoldName = getKeyNameFromItem(keyTree->currentItem());
	char *oldName;

	keyInit(&oldKey);
	
	oldName = strdup(qsoldName);
	
	keySetName(&oldKey, oldName);	
	
	registryGetKey(&oldKey);
	
	char *comment = strdup(keyComment->text());
	keySetComment(&oldKey, comment);
	
	char *value = strdup(keyValue->text());
	keySetString(&oldKey, value);
	
	
	switch (typeCombo->currentItem())
	{
		case COMBO_POS_UND:
			keySetType(&oldKey, RG_KEY_TYPE_UNDEFINED);
			//keyTree->currentItem()->setPixmap(0, undefinedIcon);
			break;
		case COMBO_POS_DIR:
			keySetType(&oldKey, RG_KEY_TYPE_DIR);
			keyTree->currentItem()->setPixmap(0, dirIcon);
			break;
		case COMBO_POS_LNK:
			keySetType(&oldKey, RG_KEY_TYPE_LINK);
			keyTree->currentItem()->setPixmap(0, linkIcon);
			break;
		case COMBO_POS_STR:
			keySetType(&oldKey, RG_KEY_TYPE_STRING);
			keyTree->currentItem()->setPixmap(0, stringIcon);
			break;
		case COMBO_POS_BIN:
			keySetType(&oldKey, RG_KEY_TYPE_BINARY);
			keyTree->currentItem()->setPixmap(0, binaryIcon);
			break;
	}
	
	int ret = registrySetKey(&oldKey);
	
	if (ret)
		perror("applying key changes");
	
	keyClose(&oldKey);
	
	registryClose();
	
	applyButton->setEnabled(false);
	revokeButton->setEnabled(false);
	showKeyValues(keyTree->currentItem());
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
	if (ignoreTextChanges)
		return;
	applyButton->setEnabled(true);
	revokeButton->setEnabled(true);
}

void MainWidgetImpl::keyTypeChanged(int id)
{
	keyAttributesChanged("");
}


/**
 * delete a key
 */
void MainWidgetImpl::deleteKey()
{
	QString name = getKeyNameFromItem(keyTree->currentItem());
	registryOpen();
	
	char *ncname = strdup(name);
	
	// delete semi disabled 
	int ret = registryRemove(ncname);
	if (ret)
		perror("removing key failed");
	delete keyTree->currentItem();
	
	
	registryClose();
}

void MainWidgetImpl::addNewDir()
{
	bool ok;
	
	QString name = QInputDialog::getText("new directory name", "enter the name for the new directory", QLineEdit::Normal, QString::null, &ok, this, "new key name inputdialog");
	
	if (ok && !name.isEmpty())
	{
		QString fullName = getKeyNameFromItem(keyTree->currentItem()) + RG_KEY_DELIM + name;
		registryOpen();
		
		::Key key;
		
		
		char *temp = strdup(fullName);
		
		keyInit(&key);

		keySetName(&key, temp);
		
		keySetType(&key, RG_KEY_TYPE_DIR);
		
		int ret = registrySetKey(&key);
		
		if (ret)
		{
			perror("adding dir");
		}
		
		keyClose(&key);
			
		registryClose();
		
		QListViewItem *item = new QListViewItem(keyTree->currentItem(), name);
		
		item->setPixmap(0, dirIcon);
		delete temp;
	}
}

void MainWidgetImpl::addNewKey()
{
	NewKeyDialogImpl *newDialog = new NewKeyDialogImpl(getKeyNameFromItem(keyTree->currentItem()), this);
	if (newDialog->exec())
	{
		registryOpen();
		
		::Key key;
		keyInit(&key);
		
		keySetType(&key, newDialog->getType());
		
		if (newDialog->getName().length())
			keySetName(&key, strdup(newDialog->getName()));
		
		if (newDialog->getComment().length())
			keySetComment(&key, strdup(newDialog->getComment()));
	
		QListViewItem *item = new QListViewItem(keyTree->currentItem(), newDialog->getName().section(RG_KEY_DELIM, -1));
		
		switch (newDialog->getType())
		{
			case RG_KEY_TYPE_STRING:
				item->setPixmap(0, stringIcon);
				break;
			case RG_KEY_TYPE_BINARY:
				item->setPixmap(0, binaryIcon);
				break;
			case RG_KEY_TYPE_LINK:
				item->setPixmap(0, linkIcon);
				break;
		}
		
		if (newDialog->getValue().length())
		{
			switch (newDialog->getType())
			{
				case RG_KEY_TYPE_STRING:
					keySetString(&key, strdup(newDialog->getValue()));
					item->setPixmap(0, stringIcon);
					break;
				case RG_KEY_TYPE_BINARY:
					keySetBinary(&key, (void *) strdup(newDialog->getValue()), newDialog->getValue().length());
					item->setPixmap(0, binaryIcon);
					break;
				case RG_KEY_TYPE_LINK:
					keySetLink(&key, strdup(newDialog->getValue()));
					item->setPixmap(0, linkIcon);
					break;
			}
		}
		int ret = registrySetKey(&key);
		
		if (ret)
		{
			perror("adding new key");
		}
		
		keyClose(&key);
		
		registryClose();
	}
}

void MainWidgetImpl::closeEvent(QCloseEvent *e)
{
	registryOpen();
	::Key width;
	::Key height;
	::Key x;
	::Key y;
	QString guiKeyPrefix =  "user/sw/regedit/gui/";
	
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
	
	MainWidget::closeEvent(e);
}


void MainWidgetImpl::checkKeyMake(::Key *key, u_int8_t type)
{	
	int ret = registryGetKey(key);
	
	if (errno == RG_KEY_RET_NOTFOUND)
	{
		keySetType(key, type);
		registrySetKey(key);
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
	registryOpen();
	char *name = strdup(getKeyNameFromItem(keyTree->currentItem()));
	char *buf;
	::Key key;
	keySetName(&key, name);
	
	registryGetKey(&key);
	
	buf = new char[keyGetDataSize(&key)];
	
	if (keyGetType(&key) == RG_KEY_TYPE_STRING)
		keyGetString(&key, buf, keyGetDataSize(&key));
	else
		keyGetBinary(&key, (void *)buf, keyGetDataSize(&key));
	
	QClipboard *cp = QApplication::clipboard();
	cp->setText(buf, QClipboard::Clipboard);
	keyClose(&key);
	registryClose();
}

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


#include "newkeydialogimpl.h"

#include <qlineedit.h>
#include <qpushbutton.h>
#include <qtextedit.h>
#include <qbuttongroup.h>
#include <iostream>

using namespace std;

NewKeyDialogImpl::NewKeyDialogImpl(QString parentKey, QWidget *parent)
: NewKeyDialog(parent, "new key dialog2", true), type(KEY_TYPE_STRING)
{
	keyName->setText(parentKey + RG_KEY_DELIM);
	
	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	
	connect(keyComment, SIGNAL(textChanged()), this, SLOT(updateComment()));
	connect(keyValue, SIGNAL(textChanged(const QString &)), this, SLOT( updateValue(const QString &)));
	connect(keyName, SIGNAL(textChanged(const QString &)), this, SLOT( updateName(const QString &)));
	connect(typeGroup, SIGNAL(clicked(int)), this, SLOT(updateType(int)));
}

QString NewKeyDialogImpl::getName()
{
	return name;
}

QString NewKeyDialogImpl::getComment()
{
	return comment;
}

QString NewKeyDialogImpl::getValue()
{
	return value;
	
}

u_int8_t NewKeyDialogImpl::getType()
{
	return type;
	
}

void NewKeyDialogImpl::updateComment()
{
	comment = keyComment->text();
	checkValues();
}

void NewKeyDialogImpl::updateName(const QString &nname)
{
	name = nname;
	checkValues();
}

void NewKeyDialogImpl::updateValue(const QString &nvalue)
{
	value = nvalue;
	checkValues();
}

void NewKeyDialogImpl::updateType(int id)
{
	this->setEnabled(true);
	switch (id)
	{
		case 0: 
			type = KEY_TYPE_BINARY;
			keyComment->setEnabled(true);
			keyValue->setEnabled(true);
			break;
		case 1:
			type = KEY_TYPE_STRING;
			keyComment->setEnabled(true);
			keyValue->setEnabled(true);
			break;
		case 2:
			type = KEY_TYPE_LINK;
			keyComment->setEnabled(false);
			keyValue->setEnabled(true);
			break;
		case 3:
			type = KEY_TYPE_DIR;
			keyComment->setEnabled(false);
			keyValue->setEnabled(false);
	}
	checkValues();
}

void NewKeyDialogImpl::checkValues()
{
	if (typeGroup->id(typeGroup->selected()))
	{
		if (keyName->text())
		{
			okButton->setEnabled(true);
		}
	}
}

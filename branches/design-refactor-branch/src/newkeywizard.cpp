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
 
#include "newkeywizard.h"
#include "keycomment.h"

#include <iostream>

using namespace std;

#include <qobject.h>
#include <qradiobutton.h>
#include <qbuttongroup.h>
#include <qpushbutton.h>
#include <qfiledialog.h>
#include <qlineedit.h>
#include <qtextedit.h>
#include <qlayout.h>

extern "C"
{
	#include <kdb.h>
}

NewKeyWizard::NewKeyWizard ( ::Key *parentKey, QWidget *parentWidget )
	: NewKeyWizardUI ( parentWidget, "new key wizard" ), parent ( parentKey ),
		binaryKeyNameSet ( false ), binaryKeyFileSet ( false )
{
	newKeys = ksNew ( );
	lineParent->setText ( parent->key );
	groupNewKeys->setColumnLayout ( 0, Qt::Vertical );
	groupNewKeys->layout()->setSpacing( 6 );
	groupNewKeys->layout()->setMargin( 11 );
	groupNewKeysLayout = new QVBoxLayout( groupNewKeys->layout() );
	groupNewKeysLayout->setAlignment( Qt::AlignTop );
	
	connect ( nextButton ( ) , SIGNAL ( clicked ( ) ), SLOT ( switchToNextPage ( ) ) );
	connect ( backButton ( ), SIGNAL ( clicked ( ) ), SLOT ( switchToLastPage ( ) ) );
	connect ( dirName, SIGNAL ( textChanged ( const QString & ) ), SLOT ( checkDirKeyName ( const QString & ) ) );
	connect ( stringKeyName, SIGNAL ( textChanged ( const QString & ) ), SLOT ( checkDirKeyName ( const QString & ) ) );
	connect ( this, SIGNAL ( selected ( const QString & ) ), SLOT ( updateSummaryPage ( const QString & ) ) );
	connect ( anotherKeyButton, SIGNAL ( clicked ( ) ), SLOT ( backToStart ( ) ) );
	connect ( chooseFile, SIGNAL ( clicked ( ) ), SLOT ( checkBinaryKeyFile ( ) ) );
	connect ( binaryKeyName, SIGNAL ( textChanged ( const QString & ) ), SLOT ( checkBinaryKeyName ( const QString & ) ) );
	connect ( groupNewKeys, SIGNAL ( pressed ( int ) ), SLOT ( keySelected( int ) ) );
	connect ( addCommentButton, SIGNAL ( clicked ( ) ), SLOT ( addComment ( ) ) ) ;
	connect ( deleteKeyButton, SIGNAL ( clicked ( ) ), SLOT ( deleteKey ( ) ) );
}

NewKeyWizard::~NewKeyWizard ( )
{
	ksDel ( newKeys );
}

void NewKeyWizard::switchToNextPage ( )
{
	if ( indexOf ( currentPage ( ) ) == 1 )
	{
		if ( radioString->isChecked ( ) )
			showPage ( WizardPage_5 );
		if ( radioBinary->isChecked ( ) )
			showPage ( WizardPage_4 );
		if ( radioDir->isChecked ( ) )
			showPage ( WizardPage_2 );
		if ( radioLink->isChecked ( ) )
			showPage ( WizardPage_3 );
		nextButton ( )->setEnabled ( false );
	}
	else
	{
		if ( indexOf ( currentPage ( ) ) == 2 ) 
		{
			::Key *tmpKey = keyNew ( QString ( parent->key ) + "/" + dirName->text ( ) , 
				KEY_SWITCH_TYPE, KEY_TYPE_DIR, 
				KEY_SWITCH_END 
				);
			//cout << "new key " << tmpKey->key << endl;
			ksAppend ( newKeys, tmpKey );
		}
		
		if ( indexOf ( currentPage ( ) ) == 5 )
		{
			::Key *tmpKey = keyNew ( QString ( parent->key ) + "/" + stringKeyName->text ( ),
				KEY_SWITCH_VALUE, stringKeyText->text ( ).ascii ( ),
				KEY_SWITCH_END );
			ksAppend ( newKeys, tmpKey );
		}
		
		if ( indexOf ( currentPage ( ) ) == 4 )
		{
			QFile file ( binaryFile->text ( ) );
			file.open ( IO_ReadOnly );
			char buf [ file.size ( ) ];
			Q_LONG bread = file.readBlock ( buf, file.size ( ) );
			cout << "read " << bread << " bytes" << endl;
			if ( file.readAll ( ).size ( ) == file.size ( ) )
				cout << "file size is: " << file.size ( ) << endl;
			if ( file.readAll ( ).data ( ) == 0 )
				cout << "shit happens" << endl;
			::Key *temp = keyNew ( QString ( parent->key ) + "/" + binaryKeyName->text ( ),
				/*KEY_SWITCH_TYPE, KEY_TYPE_BINARY, file.size ( ),
				KEY_SWITCH_VALUE, buf,*/
				KEY_SWITCH_END );
				
			
			keySetRaw ( temp, file.readAll ( ).data ( ), file.readAll ( ).size ( ) );
			keySetType ( temp, KEY_TYPE_BINARY );
			cout << keyGetValueSize ( temp ) << endl;
			ksAppend ( newKeys, temp );
		}
		
		showPage ( WizardPage_6 );
		nextButton ( )->setEnabled ( true );
	}
}

void NewKeyWizard::switchToLastPage ( )
{
	if ( indexOf ( currentPage ( ) ) == 6 )
	{
		if ( radioString->isChecked ( ) )
			showPage ( WizardPage_5 );
		if ( radioBinary->isChecked ( ) )
			showPage ( WizardPage_4 );
		if ( radioDir->isChecked ( ) )
			showPage ( WizardPage_2 );
		if ( radioLink->isChecked ( ) )
			showPage ( WizardPage_3 );
		nextButton ( )->setEnabled ( false );
	}
	else
	{
		showPage ( WizardPage );
		nextButton ( )->setEnabled ( true );
	}
}

void NewKeyWizard::checkDirKeyName ( const QString & dir )
{
	if ( dir.length ( ) )
		nextButton ( )->setEnabled ( true );
	else
		nextButton ( )->setEnabled ( false );
}

void NewKeyWizard::updateSummaryPage ( const QString & title )
{
	if ( title == "Summary" )
	{
		if ( ksGetSize ( newKeys ) )
		{
			finishButton ( ) ->setEnabled ( true );
		}
		else
		{
			finishButton ( )->setEnabled ( false );
			return;
		}
		ksRewind ( newKeys );
		::Key *temp = ksNext ( newKeys );
		
		while ( temp ) 
		{
			QObject * ch = child ( QString ( "new key radio " ) + temp->key );
			delete ch;
			temp = ksNext ( newKeys );
		}
		
		
		finishButton ( )->setEnabled ( true );
		
		ksRewind ( newKeys );
		temp = ksNext ( newKeys );
		
		while ( temp )
		{
			QRadioButton *radio = new QRadioButton ( temp->key, groupNewKeys, QString ( "new key radio " ) + temp->key );
			connect ( radio, SIGNAL ( destroyed ( ) ), SLOT ( handleDestroy ( ) ) );
			groupNewKeysLayout->addWidget ( radio );
			temp = ksNext ( newKeys );
			radio->show ( );
		}
		
		groupNewKeys->show ( );
		
	}
}

KeySet * NewKeyWizard::getNewKeys ( )
{
	return newKeys;
}

void NewKeyWizard::backToStart ( )
{
	showPage ( WizardPage );
}

void NewKeyWizard::checkBinaryKeyName ( const QString & name ) 
{
	if ( name.length ( ) )
	{
		binaryKeyNameSet = true;
		if ( binaryKeyFileSet )
			nextButton()->setEnabled ( true );
		else
			nextButton()->setEnabled ( false );
	}
	else
		binaryKeyNameSet = false;
}

void NewKeyWizard::checkBinaryKeyFile ( )
{
	QString name = QFileDialog::getOpenFileName ( 
		getenv ( "$HOME" ), 
		"All File ( * )", 
		this,
		"binary key filedialog",
		"Select a File you want to load into the key" );
	if ( name.length ( ) )
	{
		binaryFile->setText ( name );
		binaryKeyFileSet = true;
		if ( binaryKeyNameSet )
			nextButton ( )->setEnabled ( true );
		else
			nextButton ( )->setEnabled ( false );
	}
	else
		binaryKeyNameSet = false;
}

void NewKeyWizard::keySelected ( int id ) 
{
	cout << id << " selected " << endl;
	bool enable = id >= 0;
	
	addCommentButton->setEnabled ( enable );
	deleteKeyButton->setEnabled ( enable );
}

void NewKeyWizard::handleDestroy ( )
{
	keySelected ( -1 );
}

void NewKeyWizard::deleteKey ( )
{
	QString keyName =  groupNewKeys->selected ( )->text ( );
	
	KeySet *newSet = ksNew ( );
	ksRewind ( newKeys );
	::Key *key = ksNext ( newKeys );
	
	bool found = false;
	
	while ( key )
	{
		if ( QString ( key->key ) == keyName )
		{
			found = true;
		}
		else
		{
			::Key *temp = keyNew ( key->key, KEY_SWITCH_END );
			keyDup ( key, temp );
			ksAppend ( newSet , temp );
		}
		
		key = ksNext ( newKeys );
	}
	
	if ( found )
	{
		delete groupNewKeys->selected ( );
		ksDel ( newKeys );
		newKeys = newSet;
	}
	emit selected ( "Summary" );
}

void NewKeyWizard::addComment ( )
{
	KeyComment com ( this, "set key comment dialog" );
	::Key *currentKey = ksLookupByName ( newKeys, groupNewKeys->selected ( )->text ( ), 0 );
	com.comment->setText ( QString ( keyStealComment( currentKey ) ) );
	
	if ( com.exec ( ) == QDialog::Accepted )
	{
		
		if ( currentKey ) 
		{
			keySetComment ( currentKey, com.comment->text ( ) );
		}
		else
			cout << "can't find current key " << endl;
	}
}
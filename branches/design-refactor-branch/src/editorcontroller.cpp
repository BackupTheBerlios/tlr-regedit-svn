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

#include "editorcontroller.h"
#include "editorview.h"
#include "newkeywizard.h"

#include "command.h"
#include "keyaddcommand.h"
#include "keymodifycommand.h"
#include "keyremovecommand.h"

#include <errno.h>

#include <qapplication.h>
#include <qdialog.h>
#include <qlistview.h>
#include <errno.h>
#include <stdio.h>
#include <iostream>
#include <qaction.h>
#include <qpushbutton.h>
#include <qtextedit.h>
#include <qlineedit.h>

using namespace std;

EditorController::EditorController ( )
	: _current ( 0 )
{
	ignoreChanges = true;
	changedKeys = ksNew ( );
	oldKeys = ksNew ( );
	
	view = new EditorView ( this );
	qApp->setMainWidget ( view );
	
	
	
	connect ( this, SIGNAL ( notifyView ( ::Key * ) ), view, SLOT ( update ( ::Key * ) ) );
	connect ( view, SIGNAL ( keySelected ( const QString &) ), this, SLOT ( changeCurrent ( const QString & ) ) );
	
	connect ( view->newAction, SIGNAL ( activated ( ) ), SLOT ( newKey ( ) ) );
	connect ( view->deleteAction, SIGNAL ( activated ( ) ), SLOT ( delKey ( ) ) );
	connect ( view->modifyAction, SIGNAL ( activated ( ) ), SLOT ( modifyKey ( ) ) );
	
	connect ( view->undoAction, SIGNAL ( activated ( ) ), SLOT ( undo ( ) ) );
	connect ( view->redoAction, SIGNAL ( activated ( ) ), SLOT ( redo ( ) ) );
	
	connect ( view->revokeButton, SIGNAL ( clicked ( ) ), SLOT ( revokeChangedKeys ( ) ) );
	connect ( view->applyButton, SIGNAL ( clicked ( ) ), SLOT ( modifyKey ( ) ) );
	
	connect ( view->keyComment, SIGNAL ( textChanged ( ) ), SLOT ( keyFieldChanged ( ) ) );
	connect ( view->keyValue, SIGNAL ( textChanged ( const QString & ) ), SLOT ( keyFieldChanged ( const QString & ) ) );
	connect ( view->userID, SIGNAL ( textChanged ( const QString & ) ), SLOT ( keyFieldChanged ( const QString & ) ) );
	connect ( view->groupID, SIGNAL ( textChanged ( const QString & ) ), SLOT ( keyFieldChanged ( const QString & ) ) );
	connect ( view->permission, SIGNAL ( textChanged ( const QString & ) ), SLOT ( keyFieldChanged ( const QString & ) ) );
	connect ( view->keyTree, SIGNAL ( itemRenamed ( QListViewItem *, int, const QString & ) ), SLOT ( keyNameChanged( QListViewItem *, int, const QString & ) ) );
	
	
	emit notifyView ( 0 );
	ignoreChanges = false;
	
}

::Key * EditorController::currentKey ( ) const
{
	return _current;
}

QString EditorController::current ( ) const
{
	if ( _current )
		return QString ( _current->key );
	else
		return QString ( "" );
}

/**
 * 
 */
void EditorController::newKey ( )
{
	NewKeyWizard wiz( _current, view );
	if ( wiz.exec ( ) == QDialog::Accepted )
	{
		Command *cmd = new KeyAddCommand ( this, wiz.getNewKeys(), "new key command " );
		addCommand( cmd );
	} 
	else
		cout << "no" << endl;
	
}

void EditorController::delKey ( )
{
	KeySet *ks = ksNew ( );
	ksAppend ( ks, _current );
	
	addCommand( new KeyRemoveCommand ( this, ks, "keyremove command" ) );
	ksDel ( ks );
}



void EditorController::modifyKey ( )
{
	cout << "modifiying key " << endl;
	view->revokeButton->setEnabled ( false );
	view->applyButton->setEnabled ( false );
	Command *cmd = new KeyModifyCommand ( this, changedKeys, oldKeys, "key modify command" );
	addCommand ( cmd );
	ksDel ( changedKeys );
	ksDel ( oldKeys );
	changedKeys = ksNew ( );
	oldKeys = ksNew ( );
}

void EditorController::undo ( )
{
	if ( undoStack.isEmpty ( ) )
	{
		cout <<  "not able to undo" << endl;
		return;
	}
	Command * cmd = undoStack.pop ( );
	cmd->unexecute ( );
	redoStack.push ( cmd );
	
	view->undoAction->setEnabled ( !undoStack.isEmpty ( ) );
	view->redoAction->setEnabled ( true );
	
	//emit notifyView ( _current );
	changeCurrent( view->keyName ( view->keyTree->currentItem ( ) ) );
}

void EditorController::redo ( )
{
	if ( redoStack.isEmpty ( ) )
	{
		cout << "not able to redo" << endl;
		return;
	}
	Command *cmd = redoStack.pop ( );
	cmd->execute ( );
	undoStack.push ( cmd );
	
	view->redoAction->setEnabled ( !redoStack.isEmpty ( ) );
	view->undoAction->setEnabled ( true );
	//emit notifyView ( _current );
	changeCurrent( view->keyName ( view->keyTree->currentItem ( ) ) );
}

void EditorController::addCommand ( Command * cmd )
{
	redoStack.clear ( );
	undoStack.push ( cmd );
	view->undoAction->setEnabled ( true );
	view->redoAction->setEnabled ( false );
	cmd->execute ( );
	ignoreChanges  = true;
	emit notifyView( _current );
	ignoreChanges = false;
}

void EditorController::changeCurrent ( const QString & key )
{
	//cout << "changing current" << endl;
	
	::Key *temp;
	
	if ( _current )
	{
		ksRewind ( changedKeys );
		//cout << "size of changedKeys " << ksGetSize ( changedKeys ) << endl;
		if ( ksGetSize ( changedKeys ) && !ksLookupByName ( changedKeys,  _current->key, 0 ) )
		{
			//cout << "key " << _current->key << " is not in changedKeys delete it" << endl;
			keyDel ( _current );
			_current = 0;
		}
		//else
			//cout << "key " << _current->key << " is in changedKeys don't delete it" << endl;
	}
	ksRewind ( changedKeys );
	if ( temp = ksLookupByName ( changedKeys, key, 0 ) )
	{
		//cout << "key " << key << " is in changedKeys take that as current" << endl;
		_current = temp;
	}
	else
	{
		//cout << "loading fresh key " << key << endl;
		_current = keyNew ( key, KEY_SWITCH_END );
		if ( kdbGetKey ( _current ) )
		{
			perror ( "setting current key" );
			keyDel ( _current );
			_current = 0;
		}
	}
	//cout << endl << endl;
	/*if ( !_current )
	{
		_current = keyNew ( key, KEY_SWITCH_END );
		if ( kdbGetKey ( _current ) )
		{
			perror ( "setting current key" );
			cout << "error unimplemented search through the changedKeys KeySet" << endl;
			keyDel ( _current );
			_current = 0;
		}
	}
	else
	{
		keyDel ( _current );
		_current = 0;
		
		temp = ksLookupByName ( changedKeys, key, 0 );
		
		if ( temp )
		{
			_current = temp;
		}
	}
	*/
	
	
	ignoreChanges = true;
	emit notifyView ( _current );
	ignoreChanges = false;
}

void EditorController::keyFieldChanged ( )
{
	
	
	if ( ignoreChanges ) 
		return;
	
	QString name ( _current->key );
	
	view->revokeButton->setEnabled ( true );
	view->applyButton->setEnabled ( true );
	
	ksRewind ( changedKeys );
	::Key *key = ksLookupByName ( changedKeys, name, 0 );
	
	if ( !key )
	{
		//cout << "adding " << name << endl;
		::Key *old = keyNew ( name, KEY_SWITCH_END );
		keyDup ( _current, old );
		ksAppend ( oldKeys, old );
		
		::Key *neu = keyNew ( _current->key, KEY_SWITCH_END );
		keyDup ( _current, neu );
		ksAppend ( changedKeys, neu );
		
		keySetComment ( neu, view->getKeyComment ( ) );
		keySetAccess ( neu, view->getKeyAccess ( ) );
		keySetUID ( neu, view->getKeyOwner ( ) );
		keySetGID ( neu, view->getKeyGroup ( ) );
	
		if ( keyGetType ( neu ) == KEY_TYPE_STRING )
			keySetString ( neu, view->getKeyStringValue ( ) );
	}
	else
	{
		//cout << _current->key << " is already added" << endl;
		//_current = key;
		keySetComment ( key, view->getKeyComment ( ) );
		keySetAccess ( key, view->getKeyAccess ( ) );
		keySetUID ( key, view->getKeyOwner ( ) );
		keySetGID ( key, view->getKeyGroup ( ) );
	
		if ( keyGetType ( key ) == KEY_TYPE_STRING )
			keySetString ( key, view->getKeyStringValue ( ) );
		keyClose ( _current );
		keyDup ( key, _current );
	}
	
	
	//TODO whats with the binary values
}

void EditorController::keyFieldChanged ( const QString & )
{
	keyFieldChanged ( );
}

void EditorController::keyNameChanged ( QListViewItem *item, int column, const QString & newName )
{
	if ( ignoreChanges ) 
		return;
	keyFieldChanged ( );
	keySetBaseName ( _current, newName );
}

void EditorController::revokeChangedKeys ( )
{
	ksDel ( changedKeys );
	changedKeys = ksNew ( );
	view->revokeButton->setEnabled ( false );
	view->applyButton->setEnabled ( false );
	
	ksRewind ( oldKeys );
	
	::Key *key = ksNext ( oldKeys );
	
	while ( key )
	{
		changeCurrent( key->key );
		key = ksNext ( oldKeys );
	}
	
	ksDel ( oldKeys );
	oldKeys = ksNew ( );
}


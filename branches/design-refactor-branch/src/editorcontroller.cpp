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

using namespace std;

EditorController::EditorController ( )
	: _current ( 0 )
{
	view = new EditorView ( this );
	qApp->setMainWidget ( view );
	connect ( this, SIGNAL ( notifyView ( ::Key * ) ), view, SLOT ( update ( ::Key * ) ) );
	connect ( view, SIGNAL ( keySelected ( const QString &) ), this, SLOT ( changeCurrent ( const QString & ) ) );
	
	connect ( view->newAction, SIGNAL ( activated ( ) ), this, SLOT ( newKey ( ) ) );
	connect ( view->deleteAction, SIGNAL ( activated ( ) ), this, SLOT ( delKey ( ) ) );
	connect ( view->modifyAction, SIGNAL ( activated ( ) ), this, SLOT ( modifyKey ( ) ) );
	
	connect ( view->undoAction, SIGNAL ( activated ( ) ), this, SLOT ( undo ( ) ) );
	connect ( view->redoAction, SIGNAL ( activated ( ) ), this, SLOT ( redo ( ) ) );
	
	
	emit notifyView ( 0 );
	
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
}



void EditorController::modifyKey ( )
{
	
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
}

void EditorController::addCommand ( Command * cmd )
{
	redoStack.clear ( );
	undoStack.push ( cmd );
	view->undoAction->setEnabled ( true );
	view->redoAction->setEnabled ( false );
	cmd->execute ( );
	emit notifyView( _current );
}

void EditorController::changeCurrent ( const QString & key )
{
	if ( _current )
		keyDel ( _current );
		
	_current = keyNew ( key, KEY_SWITCH_END );
	if ( kdbGetKey ( _current ) )
	{
		perror ( "setting current key" );
		keyDel ( _current );
		_current = 0;
	}
	
	emit notifyView ( _current );
}

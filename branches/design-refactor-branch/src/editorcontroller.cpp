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

#include <qapplication.h>
#include <qdialog.h>
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

void EditorController::newKey ( )
{
	NewKeyWizard wiz( _current, view );
	if ( wiz.exec ( ) == QDialog::Accepted )
	{
		cout << "yes" << endl;
	} 
	else
		cout << "no" << endl;
	
}

void EditorController::delKey ( )
{
	
}

void EditorController::modifyKey ( )
{
	
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

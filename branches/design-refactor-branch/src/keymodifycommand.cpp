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

#include "keymodifycommand.h"
#include "editorcontroller.h"
#include "editorview.h"

#include <qlistview.h>
#include <qpixmap.h>
#include <qtextedit.h>
#include <qlineedit.h>
#include <qcombobox.h>

#include <iostream>
using namespace std;

KeyModifyCommand::KeyModifyCommand ( EditorController *con, KeySet *newKeys, KeySet *_oldKeys, const char *name )
 : Command ( con, newKeys, name )
{
	ksRewind ( _oldKeys );
	::Key *walker = ksNext ( _oldKeys );
	oldKeys = ksNew ( );
	
	while ( walker )
	{
		::Key *key = keyNew ( walker->key, KEY_SWITCH_END );
		keyDup ( walker, key );
		ksAppend ( oldKeys, key );
		walker = ksNext ( _oldKeys ); 
	}
}


KeyModifyCommand::~KeyModifyCommand()
{
	cout << "before delete" << endl;
	ksDel ( oldKeys );
	cout << "after delete" << endl;
}

bool KeyModifyCommand::execute()
{
	ksRewind ( subject ( ) );
	
	cout << "setting " << ksGetSize ( subject ( ) ) << " keys " << endl;

	::Key *key = ksNext ( subject ( ) );
	
	while ( key ) 
	{
		cout << "set key " << key->key << endl;
		if ( kdbSetKey ( key ) )
		{
			perror ( "(re)do modifying key" );
			return false;
		}
		key = ksNext ( subject ( ) );
	}
	return true;
}

bool KeyModifyCommand::unexecute()
{
	char buf [300];
	char name [600];
	ksRewind ( oldKeys );
	::Key *key = ksNext ( oldKeys );
	
	while ( key ) 
	{
		keyGetBaseName ( key, buf, 300 );
		keyGetName ( key, name, 600 );
		QString oldName ( buf );
		if ( kdbSetKey ( key ) )
		{
			perror ( "redo modifying key" );
		}
		keyGetBaseName ( key, buf, 300 );
		controller ( )->getView ( )->getItem( name )->setText ( 0, buf );
		key = ksNext ( oldKeys );
	}
	return true;
}



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

#include "keyremovecommand.h"
#include "editorview.h"
#include "editorcontroller.h"

#include <qlistview.h>

#include <iostream>
using namespace std;

KeyRemoveCommand::KeyRemoveCommand ( EditorController *con, KeySet *ks, const char *name ) 
 : Command ( con, ks, name)
{
	all = ksNew ( );
	
	ksRewind ( ks );
	
	::Key *key = ksNext ( ks );
	
	while ( key )
	{
		appendRecu ( key, all );
		//ksAppend ( all, key );
		key = ksNext ( ks );
	}
	item = controller ( )->getView ( )->keyTree->currentItem ( );
	parent = item->parent ( );

}



KeyRemoveCommand::~KeyRemoveCommand()
{
	
}

void KeyRemoveCommand::appendRecu ( ::Key *key, KeySet *set )
{
	KeySet *temp = ksNew ( );
	kdbGetKeyChildKeys ( key, temp, KDB_O_INACTIVE | KDB_O_DIR );
	
	::Key *tempKey = ksNext ( temp );
	while ( tempKey ) 
	{
		appendRecu ( tempKey, set );
		tempKey = ksNext ( temp );
	}
	
	::Key *dup = keyNew ( key->key, KEY_SWITCH_END );
	
	keyDup ( key, dup );
	ksAppend ( all, dup );
}

bool KeyRemoveCommand::execute( )
{
	ksRewind ( all );
	::Key *key = ksNext ( all );
	
	while ( key )
	{
		if ( kdbRemove ( key->key ) )
			perror ( QString ( key->key ) + " removing key" );
			
		key = ksNext ( all );
	}
	
	if ( item )
		parent->takeItem ( item );
	else
		cout << "item is null" << endl;
}

bool KeyRemoveCommand::unexecute( )
{
	
	ksRewind ( all );
	::Key *key = ksNext ( all );
	
	while ( key )
	{
		if ( kdbSetKey ( key ) )
		{
			perror ( "remove command undo " );
		}
		else
			cout << "undoing remove of key " << key->key << endl;
			
		key = ksNext ( all );
	}
	parent->insertItem ( item );
}




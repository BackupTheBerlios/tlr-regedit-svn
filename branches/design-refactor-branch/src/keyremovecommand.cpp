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
 : Command ( con, ks, name), all ( 0 )
{
	
}


KeyRemoveCommand::~KeyRemoveCommand()
{
	
}

bool KeyRemoveCommand::execute( )
{
	if ( all )
	{
		ksDel ( all );
	}
	
	all = ksNew ( );
		
	EditorView *view = controller ( )->getView ( );
	KeySet *ks = subject ( );
	ksRewind ( ks );
	::Key *key = ksNext ( ks );
	
	while ( key )
	{
		delRecursive ( key );	
		key = ksNext ( ks );
	}
	
	view->removeItem ( view->keyTree->currentItem ( ) );
	
}

bool KeyRemoveCommand::unexecute( )
{
	EditorView *view = controller ( )->getView ( );
	ksRewind ( all );
	::Key *key = ksNext ( all );
	
	while ( key )
	{
		//view->
		cout << "undoing remove of key " << key->key << endl;
		key = ksNext ( all );
	}
}

void KeyRemoveCommand::delRecursive ( ::Key *key )
{
	KeySet *childKeys = ksNew ( );
	kdbGetKeyChildKeys ( key, childKeys, KDB_O_DIR | KDB_O_INACTIVE );
	
	//ksAppend ( all, key );
	//cout << "doing remove of key" << key->key << endl;
	
	if ( ksGetSize ( childKeys ) )
	{
		cout << "removing " << ksGetSize ( childKeys ) << " childs " << endl;
		ksRewind ( childKeys );
		::Key *temp = ksNext ( childKeys );
		
		while ( temp )
		{
			delRecursive ( temp );
			temp = ksNext ( childKeys );
		}
	}

	cout << "doing remove of key" << key->key << endl;
	/*if ( kdbRemove ( key->key ) )
	{
		perror ( "removing key" );
	}*/
	
	
	//ksDel ( childKeys );
}


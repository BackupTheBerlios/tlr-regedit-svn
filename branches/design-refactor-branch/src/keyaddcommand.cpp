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

#include "keyaddcommand.h"
#include "editorview.h"
#include "editorcontroller.h"

#include <errno.h>
#include <string.h>
#include <qlistview.h>

#include <iostream>
using namespace std;


KeyAddCommand::KeyAddCommand ( EditorController *con, KeySet *ks, const char *name)
 : Command(con, ks, name)
{
	
}


KeyAddCommand::~KeyAddCommand()
{
	
}

bool KeyAddCommand::execute()
{
	EditorView *view = controller ( )->getView ( );
	KeySet *keys = subject ( ); 
	
	cout << "adding " << ksGetSize ( keys ) << " new Keys" << endl;
	
	view->openKeyDir ( view->keyTree->currentItem ( ) );
	
	ksRewind ( keys );
	::Key *key = ksNext ( keys );
	while ( key )
	{
		view->addItem ( view->keyTree->currentItem ( ), key );
		key = ksNext ( keys );
	}
	kdbSetKeys ( keys );
	return true;
}

bool KeyAddCommand::unexecute()
{
	EditorView *view = controller ( )->getView ( );
	KeySet *keys = subject ( );
	ksRewind ( keys );
	
	::Key *key = ksNext ( keys );
	
	while ( key )
	{
		QListViewItem * item = view->getItem( key->key );
		if ( item )
			delete item;
		kdbRemove ( key->key );
		key = ksNext ( keys );
	}
	return true;
}

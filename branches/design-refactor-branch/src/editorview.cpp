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


#include "editorview.h"
#include "keymetainfo.h"

#include <iostream>
using namespace std;

#include <qlistview.h>

extern "C"
{
	#include <kdb.h>
}

EditorView::EditorView ( EditorController *econtroller ) 
	: EditorViewUI ( 0, "the editor view", WType_TopLevel ), controller ( econtroller )
{
	updateKeyTree ( true );
	
	EditorViewUI::connect (keyTree, SIGNAL ( expanded ( QListViewItem * ) ), this, SLOT ( openKeyDir ( QListViewItem * ) ) );
	EditorViewUI::connect (keyTree, SIGNAL ( collapsed ( QListViewItem * ) ), this, SLOT ( closeKeyDir ( QListViewItem * ) ) );
	
	show();
}

void EditorView::update (  )
{
	std::cout << "update" << std::endl;
}

void EditorView::updateKeyTree ( bool firstTime )
{
	if (firstTime)
	{
		std::cout << "first time" << std::endl;
		openedKeys.clear();
		
		KeySet *roots = ksNew ( );
		kdbGetRootKeys ( roots );
		
		ksSort ( roots );
		ksRewind ( roots );
		
		::Key *k = ksNext ( roots );
		
		while ( k )
		{
			QListViewItem *item  = new QListViewItem ( keyTree, k->key);
			item->setPixmap ( 0, KeyMetaInfo::getIcon ( k ) );
			keyTree->insertItem ( item );
			openedKeys.push_back ( k->key );
			
			if (KeyMetaInfo::hasChildKeys ( k ) )
			{
				QListViewItem *dummy = new QListViewItem ( item, "dummy" );
				item->insertItem ( dummy );
			}
			
			k = ksNext ( roots );
		}
	}
	//TODO run through the openedKey list and add QListViewItems
}

void EditorView::openKeyDir ( QListViewItem *item )
{
	if ( item->firstChild ( ) -> text ( 0 ) == "dummy" )
		delete item->firstChild ( );
	else
		cout << "implementation error no dummy found" << endl;
	
	//TODO implement
}

void EditorView::closeKeyDir ( QListViewItem *item )
{
	//TODO implement
}

void EditorView::propagteKeyChange ( QListViewItem *item )
{

}

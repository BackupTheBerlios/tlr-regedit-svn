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
#include <qevent.h>

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
	
	restoreState ( );
	
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
	{
		delete item->firstChild ( );
	}
	else
		cout << "implementation error no dummy found" << endl;
	
	::Key *toOpen = keyNew ( keyName ( item ) );
	kdbGetKey ( toOpen );
	openedKeys.push_back ( toOpen->key );
	
	KeySet *childs = ksNew ( );
	kdbGetKeyChildKeys ( toOpen, childs, KDB_O_DIR | KDB_O_STATONLY | KDB_O_INACTIVE );
	ksSort ( childs );
	ksRewind ( childs );
	
	::Key *child = ksNext ( childs );
	
	while ( child )
	{
		QString absolutName ( child->key );
		QString name = absolutName.right ( absolutName.length() - absolutName.findRev ( RG_KEY_DELIM ) - 1 );

		QListViewItem *childItem = new QListViewItem ( item, name );
		childItem->setPixmap ( 0,  KeyMetaInfo::getIcon ( child ) );
		
		if ( KeyMetaInfo::hasChildKeys ( child ) )
		{
			QListViewItem *dummy = new QListViewItem ( childItem, "dummy" );
			childItem->insertItem ( dummy );
		}
		
		child = ksNext ( childs );
	}
}

void EditorView::closeKeyDir ( QListViewItem *item )
{
	
	//TODO implement
}

void EditorView::propagteKeyChange ( QListViewItem *item )
{
	//TODO implement
}

QString EditorView::keyName ( const QListViewItem * item ) const
{
	
	QString key = item->text(0);
        QListViewItem *parent = item->parent();

        while (parent != 0)
        {
                key = parent->text(0) + RG_KEY_DELIM + key;
                parent = parent->parent();
        }

        return key;

}

void EditorView::closeEvent ( QCloseEvent * e )
{
	saveState ( );
	e->accept ( );
}

void EditorView::saveState ( )
{
	QString guiKeyPrefix ( "user/sw/kdbe/gui/" );
	::Key *width = keyNew ( guiKeyPrefix + "width", KEY_SWITCH_END );
        ::Key *height = keyNew ( guiKeyPrefix + "height", KEY_SWITCH_END );
        ::Key *x = keyNew ( guiKeyPrefix + "x", KEY_SWITCH_END );
        ::Key *y = keyNew ( guiKeyPrefix + "y", KEY_SWITCH_END );
	
        

        kdbGetKey ( width );
        keySetString ( width, QString ( ).setNum ( this->width ( ) ) );
        keySetComment ( width, "This is where regedit stores the width of the window" );
        kdbSetKey ( width );
        keyDel ( width );

        kdbGetKey ( height );
        keySetString ( height, QString ( ).setNum ( this->height ( ) ) );
        keySetComment ( height, "This is where regedit stores the height of the window" );
        kdbSetKey ( height );
        keyDel ( height );

        kdbGetKey ( x );
        keySetString ( x, QString().setNum(this->x ( ) ) );
        keySetComment ( x, "This is where regedit stores the x position of the window" );
        kdbSetKey ( x );
        keyDel ( x );

        kdbGetKey ( y );
        keySetString ( y,  QString ( ).setNum ( this->y ( ) ) );
        keySetComment ( y, "This is where regedit stores the y position of the window" );
        kdbSetKey ( y );
        keyDel ( y );
}

void EditorView::restoreState ( )
{
	QString guiKeyPrefix ( "user/sw/kdbe/gui/" );
	
	char buf[300];
	
	::Key *width = keyNew ( guiKeyPrefix + "width", KEY_SWITCH_END );
	kdbGetKey ( width );
	keyGetString ( width, buf, 300 );
	int vwidth = atoi ( buf );
	
	::Key *height = keyNew ( guiKeyPrefix + "height", KEY_SWITCH_END );
	kdbGetKey ( height );
	keyGetString ( height, buf, 300 );
	int vheight = atoi ( buf );
	
	
        ::Key *x = keyNew ( guiKeyPrefix + "x", KEY_SWITCH_END );
	kdbGetKey ( x );
	keyGetString ( x, buf, 300 );
	int vx = atoi ( buf );
	
        ::Key *y = keyNew ( guiKeyPrefix + "y", KEY_SWITCH_END );
	kdbGetKey ( y );
	keyGetString ( y, buf, 300 );
	int vy = atoi ( buf );
	
	move ( vx, vy );
	resize ( vwidth, vheight );
	
	//int vy = atoi ( kdbGetKey ( §
	//TODO restore window state from registry
}

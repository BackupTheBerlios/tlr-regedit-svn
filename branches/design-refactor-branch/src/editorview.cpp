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
#include "errno.h"
#include "string.h"

#include <iostream>
using namespace std;

#include <qlistview.h>
#include <qevent.h>
#include <qsplitter.h>

extern "C"
{
	#include <kdb.h>
}

EditorView::EditorView ( EditorController *econtroller ) 
	: EditorViewUI ( 0, "the editor view", WType_TopLevel ), controller ( econtroller )
{
	updateKeyTree ( true );
	
	connect ( keyTree, SIGNAL ( expanded ( QListViewItem * ) ), this, SLOT ( openKeyDir ( QListViewItem * ) ) );
	connect ( keyTree, SIGNAL ( selectionChanged ( QListViewItem * ) ), this, SLOT ( propagateKeyChange ( QListViewItem * ) ) );
	
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
		if ( kdbGetRootKeys ( roots ) )
		{
			perror ( "opening roots" );
		}
		
		
		ksSort ( roots );
		ksRewind ( roots );
		
		::Key *k = ksNext ( roots );
		
		while ( k )
		{
			QListViewItem *item  = new QListViewItem ( keyTree, k->key);
			item->setPixmap ( 0, KeyMetaInfo::getIcon ( k ) );
			keyTree->insertItem ( item );
			
			if (KeyMetaInfo::hasChildKeys ( k ) )
			{
				QListViewItem *dummy = new QListViewItem ( item, "dummy" );
				item->insertItem ( dummy );
			}
			
			k = ksNext ( roots );
		}
	}
}

void EditorView::openKeyDir ( QListViewItem *item )
{
	if ( openedKeys.contains ( keyName ( item ) ) ) //allready opened
		return;
		
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
	if ( kdbGetKeyChildKeys ( toOpen, childs, KDB_O_DIR  | KDB_O_INACTIVE ) )
	{
		perror ( "opening key tree" );
	}
	
	keyDel ( toOpen );
	
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
	ksDel ( childs );
}

void EditorView::propagateKeyChange ( QListViewItem *item )
{
	emit keySelected ( keyName ( item ) );
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
	
	kdbSetValueByParent ( guiKeyPrefix , "width", QString ( ).setNum ( width ( ) ) );
	kdbSetValueByParent ( guiKeyPrefix , "height", QString ( ).setNum ( height ( ) ) );
	kdbSetValueByParent ( guiKeyPrefix , "x", QString ( ).setNum ( x ( ) ) );
	kdbSetValueByParent ( guiKeyPrefix , "y", QString ( ).setNum ( y ( ) ) );
	kdbSetValueByParent ( guiKeyPrefix + "splitter/", "left", QString ( ).setNum ( splitter->sizes()[0]) );
	kdbSetValueByParent ( guiKeyPrefix + "splitter/", "rigth", QString ( ).setNum ( splitter->sizes()[1]) );
}

void EditorView::restoreState ( )
{
	QString guiKeyPrefix ( "user/sw/kdbe/gui/" );
	
	char buf[300];
	
	kdbGetValueByParent ( guiKeyPrefix, "width", buf, 300 );
	int vwidth = atoi ( buf );
	
	kdbGetValueByParent ( guiKeyPrefix, "height", buf, 300 );
	int vheight = atoi ( buf );
	
	
        kdbGetValueByParent ( guiKeyPrefix, "x", buf, 300 );
	int vx = atoi ( buf );
	
        kdbGetValueByParent ( guiKeyPrefix, "y", buf, 300 );
	int vy = atoi ( buf );
	
	QValueList<int> splittersizes;
	
	kdbGetValueByParent ( guiKeyPrefix + "splitter/", "left", buf, 300 );
	splittersizes.push_back ( atoi ( buf ) );
	
	kdbGetValueByParent ( guiKeyPrefix + "splitter/", "right", buf, 300 );
	splittersizes.push_back ( atoi ( buf ) );
	
	splitter->setSizes ( splittersizes );
	
	move ( vx, vy );
	resize ( vwidth, vheight );
}

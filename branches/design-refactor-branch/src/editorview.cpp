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
#include <qlineedit.h>
#include <qtextedit.h>
#include <qradiobutton.h>
#include <qpushbutton.h>
#include <qaction.h>

//#include <magic.h>
#include "xdgmime/xdgmime.h"

extern "C"
{
	#include <kdb.h>
}

EditorView::EditorView ( EditorController *econtroller ) 
	: EditorViewUI ( 0, "the editor view", WType_TopLevel ), controller ( econtroller )
{
	
	
	connect ( keyTree, SIGNAL ( expanded ( QListViewItem * ) ), SLOT ( openKeyDir ( QListViewItem * ) ) );
	connect ( keyTree, SIGNAL ( collapsed ( QListViewItem * ) ), SLOT ( closeKeyDir ( QListViewItem * ) ) );
	connect ( keyTree, SIGNAL ( selectionChanged ( QListViewItem * ) ), SLOT ( propagateKeyChange ( QListViewItem * ) ) );
	connect ( reloadAction, SIGNAL ( activated ( ) ), SLOT ( updateKeyTree ( ) ) );
	
	restoreState ( );
	updateKeyTree ( );
	
	show();
}


void EditorView::update ( ::Key *current  )
{
	if ( !KeyMetaInfo::canWrite ( current ) )
		lockGui ( false );
	else
		lockGui ( true );
	
	clearGui ( );
	
	if ( KeyMetaInfo::canRead ( current ) ) 
		showKey ( current );
		
	updateActions ( current );
}

void EditorView::clearGui ( )
{
	radioUndefined->setChecked ( true );
	keyNam->clear ( );
	keyValue->clear ( );
	keyComment->clear ( );
	//TODO there are more
}

void EditorView::lockGui ( bool lock )
{
	keyValue->setReadOnly ( !lock );
	keyComment->setReadOnly ( !lock );
	editButton->setEnabled ( lock );
}

void EditorView::updateActions ( const ::Key *current )
{
	if ( !current )
	{
		newAction->setEnabled ( false );
		deleteAction->setEnabled ( false );
		return;
	}
	
	if ( !KeyMetaInfo::canWrite ( current ) )
	{
		newAction->setEnabled ( false );
		deleteAction->setEnabled ( false );
	}
	else
	{
		deleteAction->setEnabled ( true );
		if ( keyGetType ( current ) == KEY_TYPE_DIR )
			newAction->setEnabled ( true );
	}
}

void EditorView::showKey ( ::Key * current )
{	
	if ( current )
	{
		keyNam->setText ( QString ( current->key ) );
		keyLength->setText ( QString ( ).setNum ( keyGetValueSize ( current ) ) );
		
		switch ( keyGetType ( current ) )
		{
			case KEY_TYPE_STRING: radioString->setChecked ( true ); break;
			case KEY_TYPE_BINARY: radioBinary->setChecked ( true ); break;
			case KEY_TYPE_DIR: radioDirectory->setChecked ( true ); break;
			case KEY_TYPE_LINK: radioLink->setChecked ( true ); break;
			default:	radioUndefined->setChecked ( true );
		}
		
		if ( keyGetType ( current ) == KEY_TYPE_STRING )
		{
			char buf [ 300 ];
			if ( keyGetString ( current, buf, 300 ) ) 
			{
				keyValue->setText ( buf );
			}
		}
		else if ( keyGetType ( current ) == KEY_TYPE_BINARY || keyGetType ( current ) == KEY_TYPE_UNDEFINED ) 
		{
			char buf [ keyGetValueSize ( current ) ];
			kdbGetValue ( current->key, buf, keyGetValueSize ( current ) );
			
			const char *mime = xdg_mime_get_mime_type_for_data (buf, keyGetValueSize ( current ) );
			
			keyValue->setText ( mime );
			
			/*magic_t mag = magic_open ( MAGIC_CHECK | MAGIC_MIME | MAGIC_PRESERVE_ATIME );
			magic_load ( mag, "/usr/share/misc/file/magic" );
			
			const char * mime = magic_buffer ( mag, buf, keyGetValueSize ( current ) );
			
			//cout << "magic string is " << strlen ( mime ) << " " << mime << endl;
			
			keyValue->setText ( mime );
			//delete mime;
			
			magic_close ( mag );*/
		}
	}
	else
	{
		
	}
}

void EditorView::updateKeyTree ( )
{
	QValueList<QString> temp = openedKeys;
	openedKeys.clear ( );
	keyTree->clear ( );
		
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
		/*bool dummy = false;
		
		if  ( KeyMetaInfo::hasChildKeys ( k ) )
			dummy = true;
		addItem ( k, dummy );*/
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

	if ( temp.isEmpty ( ) )
		return;
	
	QValueList<QString>::iterator it = temp.begin ( );
	
	while ( it != temp.end ( ) )
	{
		QListViewItem * item = getItem ( *it );
		if ( *it == keyName ( item ) )
		{
			keyTree->setOpen ( item, true );
			//cout << "ok" << endl;
		}
		else
			cout << "fuck " << keyName ( item ) << " != " << *it << endl;
		/*if ( item->isOpen ( ) )
			cout << "not open" << endl;*/
		//openKeyDir ( item );
		++it;
	}
}

void EditorView::openKeyDir ( QListViewItem *item )
{
	if ( openedKeys.contains ( keyName ( item ) ) ) //allready opened
		return;
	
	if ( item->firstChild ( ) )
	{
		QListViewItem *ss = item->firstChild ( );
		
		while ( ss )
		{
			if ( ss->text ( 0 ) == "dummy" )
			{
				delete item->firstChild ( );
				//cout << "found dummy" << endl;
				break;
			}
			
			ss = ss->nextSibling ( );
		}
	}
	
	openedKeys.push_back ( keyName ( item ) );
	::Key *toOpen = keyNew ( keyName ( item ) );
	kdbGetKey ( toOpen );
	
	KeySet *childs = ksNew ( );
	if ( kdbGetKeyChildKeys ( toOpen, childs, KDB_O_DIR  | KDB_O_INACTIVE ) )
	{
		perror ( QString ( "opening key tree " ) + toOpen->key );
	}
	
	keyDel ( toOpen );
	
	ksSort ( childs );
	ksRewind ( childs );
	
	::Key *child = ksNext ( childs );
	
	while ( child )
	{
		bool dummy = false;
		
		if ( KeyMetaInfo::hasChildKeys ( child ) )
			dummy = true;
			
		addItem ( item, child, dummy );
		 
		child = ksNext ( childs );
	}
	ksDel ( childs );
}

void EditorView::closeKeyDir ( QListViewItem * item )
{
	QListViewItem *parent = item->parent ( );
	
	::Key *key = keyNew ( keyName ( item ), KEY_SWITCH_END );
	kdbGetKey ( key );
	
	delete item;
	
	QValueList<QString>::iterator it = openedKeys.begin ( );
	
	while ( it != openedKeys.end ( ) )
	{
		if ( (*it).startsWith ( key->key ) )
			openedKeys.remove ( *it );
	}
	
	if ( KeyMetaInfo::hasChildKeys ( key ) )
		addItem ( parent, key, true );
	else
		addItem ( parent, key, false );
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
 
QListViewItem * EditorView::getItem ( const QString & keyName ) const
{
	//cout << "getting " << keyName << endl;
	QListViewItem *parent = keyTree->firstChild ( );
	
	QString match;
	
	if ( !keyName.contains ( RG_KEY_DELIM ) )
	{
		//cout << "ah roooot " << parent->text ( 0 ) << " searching for " << keyName << endl;
		while ( parent->text ( 0 ) != keyName )
		{
			parent = parent->nextSibling ( );
		}
		return parent;
	}
	
	for ( int i = 0; i < keyName.contains ( RG_KEY_DELIM ) + 1; i++)
	{
		if ( !parent )
		{
			cout  << "getItem: parent is null wrong input: "  << keyName << endl;
			return 0;
		}
		while ( parent->text ( 0 ) != keyName.section ( RG_KEY_DELIM, i, i ) )
			parent = parent->nextSibling ( );
		if ( keyName.contains ( RG_KEY_DELIM )  == i ) 
			break;
		//cout << parent->text ( 0 ) << "/";
		parent = parent->firstChild ( );
	}
	//cout << endl;
	QListViewItem * item = parent;
	
	if ( item == 0 )
		cout << "getItem ( " << keyName << " ) null" << endl;
	
	return item;
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
	kdbSetValueByParent ( guiKeyPrefix + "splitter/", "right", QString ( ).setNum ( splitter->sizes()[1]) );

	int i = 0;
	
	QValueList<QString>::iterator it = openedKeys.begin ( );
	
	KeySet *toDel = ksNew ( );
	
	kdbGetChildKeys ( guiKeyPrefix + "openedKeys/", toDel, KDB_O_RECURSIVE );
	ksRewind ( toDel );
	
	::Key *key = ksNext ( toDel );
	
	while ( key )
	{
		kdbRemove ( key->key );
		key = ksNext ( toDel );
	}
	
	//kdbGetChild
	
	while ( it != openedKeys.end ( ) )
	{
		kdbSetValueByParent ( guiKeyPrefix + "openedKeys/", QString ( ).setNum ( i ), *it );
		i++;
		it++;
	}
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
	
	openedKeys.clear ( );
	
	KeySet *opened = ksNew ( );
	kdbGetChildKeys ( guiKeyPrefix + "openedKeys", opened, KDB_O_RECURSIVE );
	
	for ( size_t i = 0; i < ksGetSize ( opened ); i++)
	{
		kdbGetValueByParent ( guiKeyPrefix + "openedKeys", QString ( ).setNum ( i ), buf, 300 );
		//cout << "pushing back " << buf << endl;
		openedKeys.push_back ( buf );
	}
	ksDel ( opened );
}

void EditorView::addItem ( QListViewItem *item, ::Key *child, bool dummy )
{
	int bnlength = keyGetBaseNameSize ( child );
	char baseName[bnlength];
	keyGetBaseName ( child, baseName, bnlength );

	QListViewItem *childItem = new QListViewItem ( item, baseName );
	//childItem->setName ( child->key );
	childItem->setPixmap ( 0,  KeyMetaInfo::getIcon ( child ) );
	
	if ( dummy )
	{
		QListViewItem *dummy = new QListViewItem ( childItem, "dummy" );
		childItem->insertItem ( dummy );
	}
}

const QString EditorView::selectedKey ( )
{
	return keyName ( keyTree->currentItem ( ) );
}

void EditorView::removeItem ( QListViewItem *item )
{
	if ( openedKeys.contains ( keyName ( item ) ) )
		openedKeys.remove ( keyName ( item ) );
	delete item;
}

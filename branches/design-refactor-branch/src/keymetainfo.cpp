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

#include "keymetainfo.h"
#include <iostream>


bool KeyMetaInfo::hasChildKeys ( const QString & key )
{
	::Key *subject = keyNew ( key, KEY_SWITCH_END );
	//keySetName ( subject, name );
	kdbGetKey ( subject );
	bool hasChilds = KeyMetaInfo::hasChildKeys ( subject );
	keyDel ( subject );
	return hasChilds;
}

bool KeyMetaInfo::hasChildKeys ( const Key * key )
{
	if ( keyGetType ( key ) != KEY_TYPE_DIR )
		return false;
		
	KeySet *childs = ksNew ( );
	kdbGetKeyChildKeys ( key, childs, KDB_O_DIR | KDB_O_STATONLY | KDB_O_INACTIVE );
	size_t size = ksGetSize ( childs );
	ksDel ( childs );
	return size;
}

QPixmap KeyMetaInfo::getIcon ( const QString & key )
{
	::Key * subject = keyNew ( key, KEY_SWITCH_END );
	kdbGetKey ( subject );
	
	QPixmap temp = getIcon ( subject );
	keyDel ( subject );
	return temp;
}

QPixmap KeyMetaInfo::getIcon ( const Key * key )
{
	Key *iconKey = 0;
		
	switch ( keyGetType ( key ) )
	{
		case KEY_TYPE_DIR:
			iconKey = keyNew( QString ( SYS_PREFIX ) + "icons/dir", KEY_SWITCH_END );
			break;
		case KEY_TYPE_STRING:
			iconKey = keyNew( QString ( SYS_PREFIX ) + "icons/string", KEY_SWITCH_END );
			break;
		case KEY_TYPE_BINARY:
			iconKey = keyNew( QString ( SYS_PREFIX ) + "icons/binary", KEY_SWITCH_END );
			break;
		case KEY_TYPE_LINK:
			iconKey = keyNew( QString ( SYS_PREFIX ) + "icons/link", KEY_SWITCH_END );
			break;
		case KEY_TYPE_UNDEFINED:
			iconKey = keyNew( QString ( SYS_PREFIX ) + "icons/undefined", KEY_SWITCH_END );
			break;
		default:
			//should not happen
			std::cout << "getIconSet: should not happen" << std::endl;
	}
	
	kdbGetKey ( iconKey );
	size_t len = keyGetValueSize(iconKey);

	if (!len)
		return QPixmap();
	uchar * buf = new uchar[len];
	keyGetBinary(iconKey, buf, len);
	
	QPixmap pixm = QPixmap();
	if ( !pixm.loadFromData ( buf, len, 0, 0 ) )
	{
		std::cout << "icon not loaded" << std::endl;
		std::cout << iconKey->key << std::endl;
	}
	return pixm;
}



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
using namespace std;

#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>

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
	if ( !key )
	{
		cout << "nono theres no icon for this key" << endl;
		return NULL;
	}
		
	Key *iconKey = 0;
	
	if ( canRead ( key ) )
	{
		
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
				
				iconKey = keyNew( QString ( SYS_PREFIX ) + "icons/undefined", KEY_SWITCH_END );
				std::cout << "getIconSet: should not happen; type is: " << ( int ) keyGetType ( key ) << std::endl;
		}
	}
	else
	{
		iconKey = keyNew ( QString ( SYS_PREFIX ) + "icons/noaccess", KEY_SWITCH_END );
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

bool KeyMetaInfo::canRead ( const QString & key )
{
	::Key *subject = keyNew ( key, KEY_SWITCH_END );
	kdbGetKey ( subject );
	bool temp = canRead ( subject );
	keyDel ( subject );
	return temp;
}

bool KeyMetaInfo::canRead ( const ::Key * key )
{
	if ( !key )
		return false;
	mode_t mode = keyGetAccess ( key );
	
	if ( mode & S_IROTH )
		return true;
	
	if (mode & S_IRGRP && keyGetGID ( key ) == getgid ( ) )
		return true;
	
	if (mode & S_IRUSR && keyGetUID ( key ) == getuid ( ) )
		return true;
		
	return false;
}

bool KeyMetaInfo::canWrite ( const QString & key )
{
	::Key *subject = keyNew ( key, KEY_SWITCH_END );
	kdbGetKey ( subject );
	bool temp = canWrite ( subject );
	keyDel ( subject );
	return temp;
}

bool KeyMetaInfo::canWrite ( const ::Key * key )
{
	if ( !key )
		return false;
		
	mode_t mode = keyGetAccess ( key );
	
	if ( mode & S_IWOTH )
		return true;
	
	if (mode & S_IWGRP && keyGetGID ( key ) == getgid ( ) )
		return true;
	
	if (mode & S_IWUSR && keyGetUID ( key ) == getuid ( ) )
		return true;
		
	return false;
}

bool KeyMetaInfo::isRoot ( const QString & key )
{
	::Key *subject = keyNew ( key, KEY_SWITCH_END );
	kdbGetKey ( subject );
	bool temp = isRoot ( subject );
	keyDel ( subject );
	return temp;
}

bool KeyMetaInfo::isRoot ( const ::Key * key )
{
	KeySet *roots = ksNew ( );
	kdbGetRootKeys ( roots );
	ksRewind ( roots);
	
	::Key *oneRoot = ksNext ( roots );
	
	while ( oneRoot )
	{
		if ( !strcmp ( oneRoot->key, key->key ) )
			return true;
		oneRoot = ksNext ( roots );
	}
	return false;
}

QString KeyMetaInfo::getOwner ( const QString & key )
{
	::Key *subject = keyNew ( key, KEY_SWITCH_END );
	kdbGetKey ( subject );
	QString temp = getOwner ( subject );
	keyDel ( subject );
	return temp;
}

QString KeyMetaInfo::getOwner ( const ::Key * key )
{
	struct passwd *pwd = getpwuid ( keyGetUID ( key ) );
	QString name ( pwd->pw_name );
	//delete pwd;
	return name;
}


QString KeyMetaInfo::getGroup ( const QString & key )
{
	::Key *subject = keyNew ( key, KEY_SWITCH_END );
	kdbGetKey ( subject );
	QString temp = getGroup ( subject );
	keyDel ( subject );
	return temp;
}

QString KeyMetaInfo::getGroup ( const ::Key * key )
{
	struct group * grp = getgrgid ( keyGetGID ( key ) );
	QString group ( grp->gr_name );
	//delete grp;
	return group;
}


QString KeyMetaInfo::getAccess ( const QString & key )
{
	::Key *subject = keyNew ( key, KEY_SWITCH_END );
	kdbGetKey ( subject );
	QString temp = getAccess ( subject );
	keyDel ( subject );
	return temp;
}

QString KeyMetaInfo::getAccess ( const mode_t mode )
{
	char *readable = new char[10];

        if (S_ISDIR(mode))
                readable[0]='d';
        else
                if (S_ISLNK(mode)) readable[0]='l';
                        else readable[0]='-';
        readable[1] = mode & S_IRUSR ? 'r' : '-';
        readable[2] = mode & S_IWUSR ? 'w' : '-';
        readable[3] = mode & S_IXUSR ? 'x' : '-';
        readable[4] = mode & S_IRGRP ? 'r' : '-';
        readable[5] = mode & S_IWGRP ? 'w' : '-';
        readable[6] = mode & S_IXGRP ? 'x' : '-';
        readable[7] = mode & S_IROTH ? 'r' : '-';
        readable[8] = mode & S_IWOTH ? 'w' : '-';
        readable[9] = mode & S_IXOTH ? 'x' : '-';
        readable[10]= 0;
	
	QString tmp ( readable );
	delete readable;
	return tmp;
}

QString KeyMetaInfo::getAccess ( const ::Key * key )
{
	return KeyMetaInfo::getAccess ( keyGetAccess ( key ) );
}

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
 
#ifndef KEYMETAINFO_H
#define KEYMETAINFO_H

#include <qstring.h>
#include <qpixmap.h>

extern "C"
{
	#include <kdb.h>
}

class KeyMetaInfo
{
	public:
		static bool hasChildKeys ( const QString & key );
		static bool hasChildKeys ( const ::Key * key );
		
		static QPixmap getIcon ( const QString & key );
		static QPixmap getIcon ( const ::Key * key );
		
		static bool canRead ( const QString & key );
		static bool canRead ( const ::Key * key );
		
		static bool canWrite ( const QString & key );
		static bool canWrite ( const ::Key * key );
		
		static bool isRoot ( const QString & key );
		static bool isRoot ( const ::Key * key );
		
	private:


};

#define SYS_PREFIX "system/sw/kdbe/"
#endif

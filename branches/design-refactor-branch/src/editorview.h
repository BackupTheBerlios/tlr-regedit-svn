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
 
#ifndef EDITOR_VIEW_H
#define EDITOR_VIEW_H

#include <qvaluelist.h>
#include <qpoint.h>

extern "C"
{
	#include <kdb.h>
}

#include "editorviewui.h"

class EditorController;
class QCloseEvent;

class EditorView : public EditorViewUI
{
	Q_OBJECT
	public:
		EditorView ( EditorController * controller );
		void saveState ( );
		void restoreState ( );
		void addItem ( QListViewItem *parent, ::Key *key);
		void addRootItem ( ::Key *key );
		void removeItem ( QListViewItem *item );
		const QString selectedKey ( );
		QListViewItem *getItem ( const QString & key );
		QString keyName ( const QListViewItem *item ) const;
		
		QString getKeyStringValue ( );
		QByteArray getKeyBinaryValue ( );
		QString getKeyComment ( );
		uid_t getKeyOwner ( );
		gid_t getKeyGroup ( );
		mode_t getKeyAccess ( );
		
	public slots:
		void update ( ::Key * current );
		void openKeyDir ( QListViewItem *item );
		
		void setUser ( );
		void setGroup ( );
		void setAccess ( );
		
	signals:
		void keySelected ( const QString &key );
	
	private slots:
		void showPopupMenu ( QListViewItem *item, const QPoint & poing, int column );
		void updateKeyTree (  );
		void propagateKeyChange ( QListViewItem *item );
		void closeKeyDir ( QListViewItem *item );
		void delItemLater ( );
		
	private:
		void showKey ( ::Key *key );
		void clearGui ( );
		void lockGui ( bool lock );
		void updateActions ( const ::Key *current );
		
		
		void closeEvent ( QCloseEvent * e );
		
	private:
 		EditorController *controller;
		QListViewItem *toDel;
		QValueList<QString> openedKeys;
		//QValueList<QString> closedKeys;	//keys witch are loaded but the item is closed
		QPopupMenu *popupMenu;

		uid_t keyUID;
		gid_t keyGID;
		mode_t keyAccess;
};
 
#endif

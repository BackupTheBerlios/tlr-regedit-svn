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
 
#ifndef EDITORCONTROLLER_H
#define EDITORCONTROLLER_H

#include <qobject.h>
#include <qptrstack.h>
#include "command.h"

extern "C"
{
	#include <kdb.h>
}

class EditorView;
class QListViewItem;

class EditorController : public QObject
{
	Q_OBJECT
	
public:
	EditorController ( );
	QString current ( ) const;
	::Key * currentKey ( ) const;
	EditorView *getView ( )	{ return view; };
//	void addCommand ( Command *cmd );
	
public slots:
	void changeCurrent ( const QString & key );
	
	void newKey ( );
	void delKey ( );
	void modifyKey ( );
	
	void undo ( );
	void redo ( );
	
	void keyFieldChanged ( );
	void keyFieldChanged ( const QString & );
	void keyNameChanged ( QListViewItem *item, int column, const QString & newName );
	
	void revokeChangedKeys ( );	//called by Button revoke -> delete changedKeys and do nothing
	//void applyChangedKeys ( );	//called by Button apply -> store all keys in changedKeys

signals:
	void notifyView( ::Key *current );
	
private:
	void addCommand ( Command * );

private:
	::Key *_current;
	EditorView *view;
	QPtrStack<Command> undoStack;
	QPtrStack<Command> redoStack;
	bool ignoreChanges;
	
	//the values which were changed in the key
	KeySet *changedKeys;
	KeySet *oldKeys;
	
	QString keyComment;
	QString keyName;
	QString stringKeyValue;
	QByteArray binaryKeyValue;	//good idea how to implement binary key value changin?
	uid_t keyUser;
	gid_t keyGroup;
	mode_t keyAccess;
};

#endif

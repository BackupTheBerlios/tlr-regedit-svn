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

#include "observable.h"
#include "iobserver.h"
#include "editorviewui.h"

class EditorController;

class EditorView : public EditorViewUI, public IObserver
{
	
	public:
		EditorView ( EditorController * controller );
		void saveState ( );
		void restoreState ( );
	public slots:
		void update ( const Observable * subject );
		
	signals:
		void keySelected ( const QString &key );
	
	private slots:
		void openKeyDir ( QListViewItem *item );
		void closeKeyDir ( QListViewItem *item );
		void propagetKeyChange ( QListViewItem *item );
		
	private:
		QString keyName ( const QListViewItem &item ) const;
		void updateKeyTree ( bool firstTime = false );
		
	private:
 		EditorController *controller;
		QValueList<QString> openedKeys;
};
 
#endif

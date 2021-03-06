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

#ifndef COMMAND_H
#define COMMAND_H

#include <qobject.h>

extern "C"
{
	#include <kdb.h>
}
/**
@author Gregor Burger
*/

class EditorController;

class Command : public QObject
{
Q_OBJECT
public:
	Command(EditorController *controller, KeySet *ks, const char *name = 0);
	virtual ~Command ( );
	EditorController *controller() { return _controller; }
	KeySet *subject ( ) { return _subject; }
signals:
	void commandPerformed();
	
public slots:
	virtual bool execute() = 0;
	virtual bool unexecute() = 0;
private:
	EditorController *_controller;
	KeySet *_subject;
};

#endif

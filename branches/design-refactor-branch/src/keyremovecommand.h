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

#ifndef KEYREMOVECOMMAND_H
#define KEYREMOVECOMMAND_H

#include <command.h>
#include <qpixmap.h>

extern "C"
{
#include <kdb.h>
}

class QListViewItem;

/**
@author Gregor Burger
*/
class KeyRemoveCommand : public Command
{
Q_OBJECT
public:
    KeyRemoveCommand ( EditorController *con, KeySet *ks, const char *name );

    ~KeyRemoveCommand();
    
public slots:
	virtual bool execute();
	virtual bool unexecute();
private:
	void appendRecu ( ::Key *key, KeySet *ks );
private:
	KeySet* all;
	QListViewItem *item;
	QListViewItem *parent;
};

#endif

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

#ifndef ADDKEYCOMMAND_H
#define ADDKEYCOMMAND_H

#include <command.h>

extern "C"
{
	#include <registry.h>
}

/**
@author Gregor Burger
*/
class QListViewItem;
class NewKeyDialogImpl;

class KeyAddCommand : public Command
{
Q_OBJECT
public:
    KeyAddCommand(NewKeyDialogImpl *dialog, MainWidgetImpl *mainWidget, const char *name = 0);

    ~KeyAddCommand();
    
public slots:
	virtual bool execute();
	virtual bool unexecute();
	
private:
	::Key *key;
	NewKeyDialogImpl *newDialog;
	QListViewItem *item;
};

#endif

//
// C++ Interface: addkeycommand
//
// Description: 
//
//
// Author: Gregor Burger <gregor.burger@aon.at>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
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

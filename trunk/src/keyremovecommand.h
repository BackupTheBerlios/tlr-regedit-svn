//
// C++ Interface: keyremovecommand
//
// Description: 
//
//
// Author: Gregor Burger <gregor.burger@aon.at>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KEYREMOVECOMMAND_H
#define KEYREMOVECOMMAND_H

#include <command.h>

extern "C"
{
#include <registry.h>
}

class QListViewItem;

/**
@author Gregor Burger
*/
class KeyRemoveCommand : public Command
{
Q_OBJECT
public:
    KeyRemoveCommand(MainWidgetImpl *parent, const char *name = 0);

    ~KeyRemoveCommand();
    
public slots:
	virtual bool execute();
	virtual bool unexecute();
    
private:
	::Key *key;
	QListViewItem *item;
};

#endif

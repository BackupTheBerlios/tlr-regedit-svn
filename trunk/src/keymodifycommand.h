//
// C++ Interface: keymodifycommand
//
// Description: 
//
//
// Author: Gregor Burger <gregor.burger@aon.at>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KEYMODIFYCOMMAND_H
#define KEYMODIFYCOMMAND_H

#include "command.h"
#include <qpixmap.h>

extern "C"
{
	#include <registry.h>
}
/**
@author Gregor Burger
*/
class QListViewItem;
class KeyModifyCommand : public Command
{
Q_OBJECT
public:
    KeyModifyCommand(MainWidgetImpl *mainWidget, const char *name = 0);

    ~KeyModifyCommand();
    
public slots:
	virtual bool execute();
	virtual bool unexecute();

private:
	::Key *oldKey;
	::Key *newKey;
	QPixmap oldIcon;
	QPixmap newIcon;
	QListViewItem *item;
};

#endif

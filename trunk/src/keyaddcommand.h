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
class KeyAddCommand : public Command
{
Q_OBJECT
public:
    KeyAddCommand(::Key *key, QObject *parent = 0, const char *name = 0);

    ~KeyAddCommand();
    
public slots:
	virtual void execute();
	virtual void unexecute();
	
private:
	::Key *addedKey;
};

#endif

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

/**
@author Gregor Burger
*/
class KeyRemoveCommand : public Command
{
Q_OBJECT
public:
    KeyRemoveCommand(QObject *parent = 0, const char *name = 0);

    ~KeyRemoveCommand();

};

#endif

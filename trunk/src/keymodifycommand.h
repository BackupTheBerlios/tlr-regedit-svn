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

#include <command.h>

/**
@author Gregor Burger
*/
class KeyModifyCommand : public Command
{
Q_OBJECT
public:
    KeyModifyCommand(QObject *parent = 0, const char *name = 0);

    ~KeyModifyCommand();

};

#endif

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

/**
@author Gregor Burger
*/
class MainWidgetImpl;

class Command : public QObject
{
Q_OBJECT
public:
	Command(MainWidgetImpl *mainWidget, const char *name = 0);
	virtual ~Command();
	MainWidgetImpl *mainWidget();
signals:
	void commandPerformed();
	
public slots:
	virtual bool execute() = 0;
	virtual bool unexecute() = 0;
private:
	MainWidgetImpl *m_mainWidget;
};

#endif

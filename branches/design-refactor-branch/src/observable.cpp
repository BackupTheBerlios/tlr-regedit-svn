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

#include "observable.h"

Observable::Observable ( QObject * parent, const char * name )
	: QObject (parent, name)
{
	
}

void Observable::add ( const IObserver * observer )
{
	connect ( this, SIGNAL ( notifyAll ( const Observable * ) ), observer, SLOT ( update ( const Observable * ) ) );
	emit notifyAll( this );
	
}

void Observable::remove ( const IObserver * observer ) 
{
	disconnect ( this, SIGNAL(notifyAll ( const * Observable  ) ), observer, SLOT ( update ( const Observable * ) ) );
}
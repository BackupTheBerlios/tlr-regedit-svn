//
// C++ Interface: %{MODULE}
//
// Description: 
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KEYCOMMENT_H
#define KEYCOMMENT_H

#include <keycommentui.h>
#include <qobject.h>
#include <qstring.h>

/**
@author Gregor Burger
*/
class KeyComment : public KeyCommentUI
{
Q_OBJECT
public:
    KeyComment(QWidget * parent = 0, const char *name = 0);

    ~KeyComment();
    
private slots:
	void check ( );

};

#endif

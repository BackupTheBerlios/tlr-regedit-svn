//
// C++ Implementation: %{MODULE}
//
// Description:
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "keycomment.h"

#include <qtextedit.h>
#include <qpushbutton.h>


KeyComment::KeyComment( QWidget * parent, const char *name)
 : KeyCommentUI( parent, name, true)
{
	connect ( okButton, SIGNAL ( clicked ( ) ), SLOT ( accept ( ) ) );
	connect ( cancelButton, SIGNAL ( clicked ( ) ), SLOT ( reject ( ) ) );
	connect ( comment, SIGNAL ( textChanged ( ) ), SLOT ( check ( ) ) );
}


KeyComment::~KeyComment()
{
}

void KeyComment::check ( )
{
	okButton->setEnabled ( comment->text ( ).length ( ) );
}


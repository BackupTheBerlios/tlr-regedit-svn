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
#include "usergroupdialog.h"
#include <qlabel.h>
#include <qlistview.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <stdlib.h>
#include <qpushbutton.h>


/**
 * 
 * @param type 
 * @param parent 
 * @param name 
 * @return 
 */
UserGroupDialog::UserGroupDialog ( DialogType type, QWidget *parent, const char *name )
 : UserGroupDialogUI(parent, name), _type ( type )
{
	if ( type == User )
	{
		setCaption ( "User Selection Dialog");
		chooseLabel->setText ( " <h2> Select a User </h2>" );
		listView->setColumnText ( 0, "uid" );
		listView->setColumnText ( 1, "User" );
	}
	else
	{
		setCaption ( "Group Selection Dialog");
		chooseLabel->setText ( " <h2> Select a Group </h2>");
		listView->setColumnText ( 0, "gid" );
		listView->setColumnText ( 1, "Group" );
	}
	connect ( listView, SIGNAL (selectionChanged ( QListViewItem * ) ), SLOT ( itemSelected ( QListViewItem * ) ) ) ;
	connect ( cancelButton, SIGNAL ( clicked ( ) ), SLOT ( reject ( ) ) );
	fillListView ( );
}


UserGroupDialog::~UserGroupDialog()
{
	
}

void UserGroupDialog::fillListView ( )
{
	if ( _type == User )
	{
		struct passwd *pwd = getpwent ( );
		
		while ( pwd )
		{
			QListViewItem *item = new QListViewItem ( listView, QString ( ).setNum ( pwd->pw_uid ), pwd->pw_name );
			listView->insertItem ( item );
			pwd = getpwent ( );
		}
	}
	else
	{
		struct group *grp = getgrent ( );
		
		while ( grp )
		{
			QListViewItem *item = new QListViewItem ( listView, QString ( ).setNum ( grp->gr_gid ), grp->gr_name );
			listView->insertItem ( item );
			grp = getgrent ( );
		}
	}
}

void UserGroupDialog::itemSelected ( QListViewItem *item )
{
	if ( _type == User )
	{
		uid = (uid_t) atoi ( item->text ( 0 ) );
		user = item->text ( 1 );
	}
	else
	{
		gid = (gid_t) atoi ( item->text ( 0 ) );
		group = item->text ( 1 );
	}
	accept ( );
}

uid_t UserGroupDialog::getUID ( )
{
	return uid;
}

gid_t UserGroupDialog::getGID ( )
{
	return gid;
}

QString UserGroupDialog::getUser ( )
{
	return user;
}

QString UserGroupDialog::getGroup ( )
{
	return group;
}



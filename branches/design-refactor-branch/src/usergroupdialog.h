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
#ifndef USERGROUPDIALOG_H
#define USERGROUPDIALOG_H
#include <sys/types.h>

#include <usergroupdialogui.h>

class QListViewItem;
/**
@author Gregor Burger
*/
class UserGroupDialog : public UserGroupDialogUI
{
Q_OBJECT
public:
	enum DialogType { Group, User };
    UserGroupDialog(DialogType type, QWidget *parent = 0, const char *name = 0);

    ~UserGroupDialog();
    
    uid_t getUID ( );
    gid_t getGID ( );
    QString getUser ( );
    QString getGroup ( );
    
private slots:
	void itemSelected ( QListViewItem * item );
	void fillListView ( );
	
private:
	DialogType _type;
	uid_t uid;
	gid_t gid;
	QString user;
	QString group;
};

#endif

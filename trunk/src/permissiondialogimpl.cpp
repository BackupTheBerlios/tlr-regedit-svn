//
// C++ Implementation: permissiondialogimpl
//
// Description: 
//
//
// Author: Gregor Burger <gregor.burger@aon.at>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "permissiondialogimpl.h"

#include "sys/stat.h"
#include "stdlib.h"

#include <qpushbutton.h>
#include <qcheckbox.h>


PermissionDialogImpl::PermissionDialogImpl(mode_t mode, QWidget *parent, const char *name)
 : PermissionDialog(parent, name, true), mode(mode)
{
	connect(okButton, SIGNAL(pressed()), this, SLOT(accept()));
	connect(cancleButton, SIGNAL(pressed()), this, SLOT(reject()));
	
	ur->setChecked(mode & S_IRUSR);
	uw->setChecked(mode & S_IWUSR);
	ux->setChecked(mode & S_IXUSR);
	gr->setChecked(mode & S_IRGRP);
	gw->setChecked(mode & S_IWGRP);
	gx->setChecked(mode & S_IXGRP);
	otr->setChecked(mode & S_IROTH);
	ox->setChecked(mode & S_IWOTH);
	ox->setChecked(mode & S_IXOTH);
}


PermissionDialogImpl::~PermissionDialogImpl()
{
}



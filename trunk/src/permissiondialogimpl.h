//
// C++ Interface: permissiondialogimpl
//
// Description: 
//
//
// Author: Gregor Burger <gregor.burger@aon.at>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PERMISSIONDIALOGIMPL_H
#define PERMISSIONDIALOGIMPL_H

#include <permissiondialog.h>
#include <sys/types.h>
/**
@author Gregor Burger
*/
class PermissionDialogImpl : public PermissionDialog
{
Q_OBJECT
public:
	PermissionDialogImpl(mode_t mode, QWidget *parent = 0, const char *name = 0);

	~PermissionDialogImpl();
	
private:
	mode_t mode;

private slots:
	
};

#endif

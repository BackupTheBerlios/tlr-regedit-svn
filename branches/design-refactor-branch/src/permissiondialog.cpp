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
 
#include "permissiondialog.h"

#include "sys/stat.h"
#include "stdlib.h"

#include <qpushbutton.h>
#include <qcheckbox.h>


PermissionDialog::PermissionDialog(mode_t mode, QWidget *parent, const char *name)
 : PermissionDialogUI(parent, name, true), mode(mode)
{
	connect(okButton, SIGNAL(pressed()), this, SLOT(translateMode()));
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


PermissionDialog::~PermissionDialog()
{
	
}

void PermissionDialog::translateMode ( )
{
	if (ur->isChecked()) mode |= S_IRUSR; else mode &= ~S_IRUSR;
	if (uw->isChecked()) mode |= S_IWUSR; else mode &= ~S_IWUSR;
	if (ux->isChecked()) mode |= S_IXUSR; else mode &= ~S_IXUSR;
	
	if (gr->isChecked()) mode |= S_IRGRP; else mode &= ~S_IRGRP;
	if (gw->isChecked()) mode |= S_IWGRP; else mode &= ~S_IWGRP;
	if (gx->isChecked()) mode |= S_IXGRP; else mode &= ~S_IXGRP;
	
	if (otr->isChecked()) mode |= S_IROTH; else mode &= ~S_IROTH;
	if (ow->isChecked()) mode |= S_IWOTH; else mode &= ~S_IWOTH;
	if (ox->isChecked()) mode |= S_IXOTH; else mode &= ~S_IXOTH;
}

mode_t PermissionDialog::getAccess ( )
{
	return mode;
}

#include "regedit_globals.h"

#include <grp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>

using namespace std;

void checkKeyMake(Key *key, u_int8_t type)
{	
	int ret = registryGetKey(key);
	
	if (errno == RG_KEY_RET_NOTFOUND)
	{
		keySetType(key, type);
		registrySetKey(key);
	}	
}

QString getIconDir()
{
	registryOpen();
	::Key iconDir;
	
	keyInit(&iconDir);
	
	keySetName(&iconDir, "system/sw/regedit/gui/iconDir");
	
	int ret = registryGetKey(&iconDir);
	
	if (ret)
	{
		//cout << "using user namespace iconDir" << endl;
		keyClose(&iconDir);
		keyInit(&iconDir);
		keySetName(&iconDir, "user/sw/regedit/gui/iconDir");
		checkKeyMake(&iconDir, RG_KEY_TYPE_STRING);
	}
	else
		//cout << "using system namespace iconDir" << endl;
	
	if (keyGetDataSize(&iconDir))
	{	
		char *buf = new char[keyGetDataSize(&iconDir)];
		keyGetString(&iconDir, buf, keyGetDataSize(&iconDir));
		
		keyClose(&iconDir);
		registryClose();
		return QString(buf);
	}

	keyClose(&iconDir);
	registryClose();
	return QString::null;

}


bool checkForPermission(int access, mode_t mode, uid_t user, gid_t group)
{
	uid_t uid = getuid();
	size_t ng = getgroups(0, 0);
	gid_t groups[ng];
	
	if (user == 0)
		return true;
	
	if (mode & access)
	{
		if (user == uid)
			return true;
	}
	
	if (mode & access)
	{
		for (unsigned int i = 0; i < ng; i++)
		{
			if (group == groups[i])
				return true;
		}
	}
	
	return false;
}


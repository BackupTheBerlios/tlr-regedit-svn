#include "regedit_globals.h"

#include <grp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>

using namespace std;

void checkKeyMake(Key *key, u_int8_t type)
{	
	int ret = kdbGetKey(key);
	
	if (errno == KDB_RET_NOTFOUND)
	{
		keySetType(key, type);
		kdbSetKey(key);
	}	
}

QString getIconDir()
{
	kdbOpen();
	::Key iconDir;
	
	keyInit(&iconDir);
	
	keySetName(&iconDir, "system/sw/regedit/gui/iconDir");
	
	int ret = kdbGetKey(&iconDir);
	
	if (ret)
	{
		//cout << "using user namespace iconDir" << endl;
		keyClose(&iconDir);
		keyInit(&iconDir);
		keySetName(&iconDir, "user/sw/regedit/gui/iconDir");
		checkKeyMake(&iconDir, KEY_TYPE_STRING);
	}
	else
		//cout << "using system namespace iconDir" << endl;
	
	if (keyGetDataSize(&iconDir))
	{	
		char *buf = new char[keyGetDataSize(&iconDir)];
		keyGetString(&iconDir, buf, keyGetDataSize(&iconDir));
		
		keyClose(&iconDir);
		kdbClose();
		return QString(buf);
	}

	keyClose(&iconDir);
	kdbClose();
	return QString::null;

}


bool checkForPermission(enum access mode, ::Key *key)
{
	uid_t keyOwner = keyGetUID(key);
	uid_t uid = getuid();
	size_t ng = getgroups(0, 0);
	gid_t groups[ng];
	
	if (uid == 0)
		return true;
	
	/*if (mode & access)
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
	}*/
	
	return false;
}


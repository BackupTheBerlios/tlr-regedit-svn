#include "regedit_globals.h"
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

#include "regedit_globals.h"

void checkKeyMake(Key *key, u_int8_t type)
{	
	int ret = registryGetKey(key);
	
	if (errno == RG_KEY_RET_NOTFOUND)
	{
		keySetType(key, type);
		registrySetKey(key);
	}	
}

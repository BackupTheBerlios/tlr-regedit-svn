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

#include <iostream>

using namespace std;

extern "C"
{
	#include <registry.h>
}

void list(Key *key)
{
	KeySet keys;
	
	ksInit(&keys);
	
	registryGetChildKeys(key->key, &keys, RG_O_DIR);
	
	Key *worker = keys.start;
	
	while (worker)
	{
		cout << worker->key << " flags :" << keyGetFlag(worker) <<  endl;
		list(worker);
		worker = worker->next;
	}
}

int main()
{
	registryOpen();
	
	KeySet ks;
	ksInit(&ks);
	
	registryGetRootKeys(&ks);
	
	Key *worker = ks.start;
	
	while (worker)
	{
		cout << worker->key << endl;
		list(worker);
		worker = worker->next;
	}
	
	registryClose();
}

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

extern "C"
{
	#include <registry.h>
}

#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
/**
 * 
 * @param argc 
 * @param argv 
 * @return 
 */
#include <stdio.h>
#include <errno.h>


using namespace std;

int main(int argc, char *argv)
{

	//for (int i = 0; i < 100; i++)
	{
		registryOpen();
		
		::Key key;
		keyInit(&key);
		
		//cout << i << "th test : ";
		
		//if (i % 2)
		{
			char * name = "user/ex/temp";
			keySetName(&key, name);
			if (registryStatKey(&key))
				perror("stating key");
			
			if (registryGetKey(&key))
				perror("getting key");
				
			mode_t mode = keyGetAccess(&key);
			
			if (mode & S_IRUSR)
				cout << "can read" << endl;
			if (mode & S_IWUSR)
				cout << "can write" << endl;
			if (mode & S_IXUSR)
				cout << "can exec" << endl;
				
			if (mode & S_IXGRP)
				cout << "group can execute" << endl;
			
			if (keyIsInitialized(&key))
				cout << "initialized" << endl;
			//cout << registryGetKey(&key) << keyGetFlag(&key)<< endl;
		}
		/*else
		{
			key.key = "user/example";
			cout << registryGetKey(&key) << endl;
		}*/
		
		registryClose();
	}
}

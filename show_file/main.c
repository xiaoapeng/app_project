
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wchar.h>
#include <string.h>

#include <encoding_manager.h>
#include <disp-manager-core.h>




int main(void)
{
	int i;
	char str[] = "小明同学？";
	wchar_t *pdwStr;
	if ( CodeInit() )
		return -1;
		
	if ( DisplayInit() )
	{
		CodeExit();
		return -1;
	}	
	

	
	DisplayExit();
	CodeExit();
	return 0;
}





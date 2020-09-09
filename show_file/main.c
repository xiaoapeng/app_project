#include <disp-manager-core.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



int main(void)
{
	int i,err=0;
	struct DispDeviceInfo*  pt_FBDevInfo;
	if ( DisplayInit() )
		return -1;
	CleanScreen(0xffffff);
	pt_FBDevInfo=GetDispInfo("fb-dev");
	if (pt_FBDevInfo == (struct DispDeviceInfo*)-1)
	{
		printf("fb-dev information not available\n");
		return -1;
	}
	printf("xres=%lu\n",pt_FBDevInfo->dwXres);
	printf("yres=%lu\n",pt_FBDevInfo->dwYres);
	printf("width=%lu\n",pt_FBDevInfo->dwWidth);
	printf("height=%lu\n",pt_FBDevInfo->dwHeight);
	printf("bpp=%lu\n",pt_FBDevInfo->dwBPP);
	for(i=0;i<pt_FBDevInfo->dwXres;i++) 
	{
		err=PixelDisplay(i,pt_FBDevInfo->dwYres/2,0xff0000,GENETAL_COORDINATE);
		if(err)
		{
			return -1;
		}
	}
	DisplayExit();
	return 0;
}





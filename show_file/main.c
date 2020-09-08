#include <disp-manager-core.h>
#include <stdio.h>


int main(void)
{
	int i;
	struct DispDeviceInfo*  pt_FBDevInfo;
	DisplayInit();
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
		PixelDisplay(i,pt_FBDevInfo->dwYres/2,0xff0000,GENETAL_COORDINATE);
		PixelDisplay(i,pt_FBDevInfo->dwYres/2+1,0xff0000,GENETAL_COORDINATE);
	}
	DisplayExit();
	return 0;
}





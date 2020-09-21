
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wchar.h>
#include <string.h>

#include <encoding_manager.h>
#include <disp-manager-core.h>
#include <fonts_manager.h>

#define FB_NAME "fb-dev"


int Init(void)
{
	int error;
	error=CodeInit();
	if(error)
		return -1;
	error=FontsInit();
	if(error)
		return -1;
	error=DisplayInit();
	if(error)
		return -1;
	return 0;
}
void Exit(void)
{
	CodeExit();
	FontsExit();
	DisplayExit();
}

int main(void)
{
	char 					str[] = "大";
	wchar_t*				strTarget;
	struct RequirInfo   	ptRequirInfo;
	struct DispDeviceInfo*	ptDispDeviceInfo;
	int						FontDesc;	
	int						FontDesc2;
	struct ImageMap*		ptImage;
	int						error;
	int x,y;
	/* 1.初始化各模块 */
	if(Init())
		return -1;


	/* 2.获取屏幕各项参数 */
	ptDispDeviceInfo = GetDispInfo(FB_NAME);
	if(ptDispDeviceInfo == NULL)
		return -1;
	
	
	/* 3.设置字体模块结构体，获取一个字体位图通道描述符 */
	ptRequirInfo.CodingFormat = "utf16-le";
	ptRequirInfo.FontType = "simsun";	/*宋体*/
	ptRequirInfo.iAngle = 0 ;
	ptRequirInfo.idwPT  = 10;
	ptRequirInfo.udwPhysHeight = 100;//ptDispDeviceInfo->dwHeight;
	ptRequirInfo.udwPhysWidth  = 250;//ptDispDeviceInfo->dwWidth;
	ptRequirInfo.udwXres	   = ptDispDeviceInfo->dwXres;
	ptRequirInfo.udwYres	   = ptDispDeviceInfo->dwYres;
	printf("udwPhysHeight=%lu , dwWidth=%lu\n",ptDispDeviceInfo->dwHeight,ptDispDeviceInfo->dwWidth);	
	printf("dwXres=%lu , dwYres=%lu\n",ptDispDeviceInfo->dwXres,ptDispDeviceInfo->dwYres);
	
	FontDesc = Fonts_open(&ptRequirInfo);	
	if(FontDesc==-1)
		return -1;
	/* 4.进行编码转化 */
	strTarget = CodeConversion(CODE_UTF8, CODE_UTF16_LE,str,strlen(str),NULL);
	
	if(strTarget==NULL)
	{
		printf("main:Convert defeat\n");
		return -1;
	}
	printf("strTarget[0] = 0x%08x\n",strTarget[0]);
	printf("code:%s\n",CodeGuess(str,strlen(str)) == CODE_UTF8 ? "is utf8": "not's utf8");


	
	/* 5.获取位图 */
	ptImage = Fonts_getmap(FontDesc,strTarget[0]);
	if(ptImage == NULL)
	{
		return -1;
	}

	/* 6.打印在屏幕上面 */
	/* 6.1 清屏 */	
	error = CleanScreen(0x000000);
	if(error)
		return -1;
	/* 6.2 打印像素 */
	

	for(x=0;x<GetImageWidth(ptImage);x++)
	{
		for(y=0;y<GetImageHeight(ptImage);y++)
		{
			int var;
			var=GetImageBit(ptImage,x,y);
			if(var)
			{
				error=PixelDisplay(x, GetImageHeight(ptImage)-y-1, 0xffffff,CARTESIAN_COORDINATE );
				if(error)
					return -1;
			}
		}
	}
	Fonts_putmap(ptImage);
	Fonts_close(FontDesc);

	
	ptRequirInfo.idwPT  = 100;
	FontDesc2 = Fonts_open(&ptRequirInfo);	
	if(FontDesc2==-1)
		return -1;

	ptImage = Fonts_getmap(FontDesc2,strTarget[0]);
	if(ptImage == NULL)
	{
		return -1;
	}

	for(x=0;x<GetImageWidth(ptImage);x++)
	{
		for(y=0;y<GetImageHeight(ptImage);y++)
		{
			int var;
			var=GetImageBit(ptImage,x,y);
			if(var)
			{
				error=PixelDisplay(x, GetImageHeight(ptImage)-y-1, 0xffffff,CARTESIAN_COORDINATE );
				if(error)
					return -1;
			}
		}
	}
	Fonts_putmap(ptImage);
	Fonts_close(FontDesc2);

	
	/* 7.清理工作 */
	//Fonts_putmap(ptImage);
	//Fonts_close(FontDesc);
	CodeDWFree(strTarget);
	
	Exit();
	return 0;
}





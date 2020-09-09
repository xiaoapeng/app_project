/*	根据文字大小、字体、
 *	来获取位图
 *	PT	:字体大小单位	72个点等于一英寸
 *	DPI	:每英寸能打印出多少像素
 *	
 *
 */
#include <fonts_manager.h>




#define SCREEN_USE		1
#define SCREEN_NO_USE	0
#define MODULE_NAME	"fonts-core"
#define SCREEN_NUM 		4
static struct OpenInfo ScreenDev[SCREEN_NUM];

static int LookingForDesc(void)
{
	int i;
	for(i=0;i<SCREEN_NUM;i++)
	{
		if(ScreenDev.iUseFlag == SCREEN_NO_USE)
			return i;
	}
	return -1;
}


/*
 *	打开一个字体获取通道
 *	参数: 
 *		ptOpenInfo	 关于打开通道的信息
 *	返回值：
 *		成功返回通道描述符
 *		失败返回 -1
 */
int Fonts_open(struct OpenInfo * ptOpenInfo)
{
	int iDesc;
	if(ptOpenInfo == NULL)
	{
		printf(MODULE_NAME": Use a null pointer\n")
		return -1;
	}
	iDesc = LookingForDesc();
	if(iDesc == -1)
	{
		printf(MODULE_NAME": There are no descriptors available\n");
		return -1;
	}
	ScreenDev[iDesc] = *ptOpenInfo
	ScreenDev[iDesc].iUseFlag = SCREEN_USE;
	return iDesc;
}

void Fonts_close(int Desc)
{
	ScreenDev[iDesc].iUseFlag = SCREEN_NO_USE;
}


/* 以下是提供给模块的函数 */

/*****************************************
 *	注册一个字体通道
 *	
 *
 *
 *
 *****************************************/
int  RegisteredFontsChannel();
{


}















int FontsInit(void)
{
	

}

int FontsExit(void)
{


}






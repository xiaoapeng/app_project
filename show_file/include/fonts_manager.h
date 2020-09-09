
#ifndef __NTS_MANAGER__H_
#define __NTS_MANAGER__H_






/*	根据屏幕信息来推算出一个点等于多少个像素
 *		iUseFlag	使用标准			1为使用
 *		dwXres:		x方向像素数量
 *		dwYres		y方向像素数量
 *		dwWidth		x方向屏幕大小 以毫米为单位
 *		udwHeight	y方向屏幕大小
 *		udwPT		字体大小 以点为单位
 *		FontType	字体
 */
struct OpenInfo{
	int iUseFlag;
	/* 像素信息 */
	unsigned long  udwXres;
	unsigned long  udwYres;	
	/* 物理尺寸 */
	unsigned long  udwWidth;		
	unsigned long  udwHeight;
	unsigned long  udwPT;

	char * CodingFormat;
	int 	FontType	;
};


struct FontOps {
	int ChannelCtl(unsigned long  udwPT, int FontType)
	
	
}

/***************************************************************
 *	模块注册通道时使用的结构体
 *  成员：
 *		SupportFontTypeS:	支持的字体类型
 *		SupportPixelS	:	支持多少的像素		为了	（支持字库文件）
 *		CodingFormatS	:	支持哪些编码的转化
 *  	Ops				:	下层提供的函数接口
 ***************************************************************/

struct FontsChannel{
	int *SupportFontTypeS;
	int *SupportPixelS;
	char ** CodingFormatS;
	struct FontOps Ops;
};






#endif	/* __NTS_MANAGER__H_ */






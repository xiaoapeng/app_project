
#ifndef __NTS_MANAGER__H_
#define __NTS_MANAGER__H_
#include <stdint.h>
#include <ulist.h>

/* 支持所有字号 */
#define ALL_SIZE 	(-1)
/**************************************************************	
 * OPEN时需要设置的结构体
 *		dwXres:				x方向像素数量
 *		dwYres				y方向像素数量
 *		udwPhysWidth		x方向屏幕大小 以毫米为单位
 *		udwPhysHeight		y方向屏幕大小
 *		udwPT				字体大小 以点为单位
 *		CodingFormat		编码格式
 *		FontType			字体样式
 **************************************************************/

struct RequirInfo{
	unsigned long  	udwXres;
	unsigned long  	udwYres;
	/* 物理尺寸 */
	unsigned long  	udwPhysWidth;	
	unsigned long  	udwPhysHeight;
	int  			idwPT;	
	char* 			CodingFormat;
	char* 			FontType;
};

/* 
 *	Fonts_ctrl函数的控制宏
 *	CMD_CTRL_CODE	改变该描述符的编码格式
 *	CMD_CTRL_PT		改变该描述符的字体大小
 *	CMD_CTRL_FONT	改变该描述符的字体
 */

#define CMD_CTRL_CODE	(1)
#define CMD_CTRL_PT		(2)
#define CMD_CTRL_FONT	(3)

#define ctrl_pt(WordSize)	(WordSize)
#define ctrl_code(CodePtr)	((intptr_t)(CodePtr))
#define ctrl_font(FontPtr) 	((intptr_t)(FontPtr))

#define ctrl_to_pt(WordSize)	(WordSize)
#define ctrl_to_code(CodePtr)	((char*)(CodePtr))
#define ctrl_to_font(FontPtr) 	((char*)(FontPtr))


struct FontOps {
	struct ImageMap* (*FontsGetmap)(int Desc,wchar_t Code);
	void (*FontsPutmap)(struct ImageMap* ptImageMap);
};



/***************************************************************
 *	模块注册通道时使用的结构体
 *  成员：
 *		SupportFontTypeS:	支持的字体类型	"songti" or "heiti"
 											or "weiruanyahei"
 *		SupportPixel	:	支持多少的像素		为了	（为了支持字库文件）
 *		SupportPT		:	支持多大的字体
 *		CodingFormatS	:	支持哪些编码的转化
 *  	Ops				:	下层提供的函数接口
 ***************************************************************/

struct FontsChannel{
	char 	*name;
	char 	*SupportFontTypeS;
	int 	SupportPixel;
	int 	SupportPT;
	char 	** CodingFormatS;
	struct 	FontOps *Ops;
	struct list_head  ChannelNode;
};


typedef unsigned long mapU32_t;


/**************************************************************
 *	点阵描述结构体
 *	image	点阵位图
 *	Width	点阵宽
 *	Height	点阵高
 *		点阵的大小 = ( Width * Height + 31) & 0xFFFF FFE0 / 32
 **************************************************************/
struct ImageMap{
	mapU32_t 	  	*image;
	unsigned long  	Width;		
	unsigned long  	Height;
	struct FontsChannel *ptFontsChannel;
};

static inline int __GetImageBit(mapU32_t *image, int pos)
{	
	mapU32_t Base = image[pos>>5];
	/* 去除高位 */
	pos &= 0xFFFFFFE0;
	Base = Base << (31-Base);
	Base = Base >> 31;
	return Base;
}	


static inline void __SetImageBit(mapU32_t *image, int pos,int var)
{	
	int index;
	mapU32_t dwValue;
	index = pos >> 5;
	dwValue = image[index];
	pos &= 0xFFFFFFE0;
	image[index] = var ? dwValue | (1 << pos)
					   : dwValue & (~ (1 << pos)) ;
}


#define GetImageBit(ptImageMap,x,y)  	__GetImageBit((ptImageMap)->image\
										,(x)*(ptImageMap)->Width+(y))
#define SetImageBit(ptImageMap,x,y,var)	__SetImageBit((ptImageMap)->image\
										,(x)*(ptImageMap)->Width+(y),var)
#define GetImageWidth(ptImageMap)	((ptImageMap)->Width)
#define GetImageHeight(ptImageMap)	((ptImageMap)->Height)







#endif	/* __NTS_MANAGER__H_ */






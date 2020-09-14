

#include <stdio.h>
#include <string.h>
#include <math.h>

#include <config.h>
#include <fonts_manager.h>
#include <ft2build.h>
#include <freetype/ftglyph.h>


#define MODULE_NAME "freetype"

static FT_Library 		g_tLibrary;
static FT_Face 			g_tFace;
static FT_GlyphSlot 	g_tSlot;
static double	  		g_dAngle;
static FT_Matrix  		gt_matrix;				  /* 变换矩阵 */ 
static FT_Vector 		pen; /* 非变换原点 */

/* 渲染到位图结构 */
static void FreetypeDraw_bitmap( FT_Bitmap*  bitmap,
             struct ImageMap* ptImageMap)
{
  FT_Int  x, y;
  FT_Int  x_max = GetImageWidth(ptImageMap);
  FT_Int  y_max = GetImageHeight(ptImageMap);
	for ( x=0; x < x_max; x++ )
	{
		for ( y=0; y < y_max; y++ )
		{
			SetImageBit(ptImageMap, x, y,bitmap->buffer[y*bitmap->width+x] );
		}
	}
}



/*****************************************
 *	配置函数
 *	参数：
 *		Desc: 通道描述符 通过描述符来获取信息
 *	返回值：
 *		成功返回0 失败返回 -1
 *****************************************/
int FreetypeFontsConfig(int Desc)
{
	int iError;
	char filename[20]; 
	/* 显示矢量字体 */
	iError = FT_Init_FreeType(&g_tLibrary );			   /* initialize library */
	/* error handling omitted */
	if (iError)
	{
		printf(MODULE_NAME": FreetypeFontsConfig failed\n");
		return -1;
	}
	strcpy(filename,GetInfoFontType(Desc));
	strcat(filename, ".ttc");
	iError = FT_New_Face(g_tLibrary, filename, 0, &g_tFace); /* create face object */
	if (iError)
	{
		printf(MODULE_NAME": FT_Init_FreeType failed\n");
		return -1;
	}
	g_tSlot = g_tFace->glyph;
	iError = FT_Set_Char_Size(g_tFace,GetInfoPT(Desc)*64,GetInfoPT(Desc)*64, 
									GetInfoWDip(Desc),GetInfoHDip(Desc));
	if (iError)
	{
		printf(MODULE_NAME": FT_Set_Pixel_Sizes failed \n");
		return -1;
	}
	/* 注意	  这是笛卡尔坐标系   	      我们从高一点的位置开始渲染*/
	pen.x = 0*64;
	pen.y = GetInfoPT(Desc)*64;	
	g_dAngle = ( ((double)GetInfoAngle(Desc)) / 360 ) * 3.14159 * 2;	  /* use 25 degrees 	*/
	gt_matrix.xx = (FT_Fixed)( cos( g_dAngle ) * 0x10000L );
	gt_matrix.xy = (FT_Fixed)(-sin( g_dAngle ) * 0x10000L );
	gt_matrix.yx = (FT_Fixed)( sin( g_dAngle ) * 0x10000L );
	gt_matrix.yy = (FT_Fixed)( cos( g_dAngle ) * 0x10000L );
	FT_Set_Transform( g_tFace, &gt_matrix, &pen );
	return 0;
}



static struct ImageMap* FreetypeFontsGetmap(int Desc,wchar_t Code)
{
	struct ImageMap*	ptImageMap;
	int 				error;
	/* 装载字形图像到字形槽（将会抹掉先前的字形图像） */
	error = FT_Load_Char( g_tFace,Code, FT_LOAD_RENDER );
	if(error)
		return NULL;
	ptImageMap = FontsAllocMap(g_tSlot->bitmap.width, g_tSlot->bitmap.rows);
	if(ptImageMap == NULL)
		return NULL;	
	FreetypeDraw_bitmap(&g_tSlot->bitmap,ptImageMap);						
	return  ptImageMap;
}

static void FreetypeFontsPutmap(struct ImageMap* ptImageMap)
{
	FontsFreeMap(ptImageMap);
}


static struct	FontOps FreetypeOps = {
	.FontsGetmap = FreetypeFontsGetmap,
	.FontsPutmap = FreetypeFontsPutmap,
	.FontsConfig = FreetypeFontsConfig,
};

static char	*FreetypeCodingFormatS[]={
	"utf16-le",NULL,
};

static struct FontsChannel FontsChannel_songti = {
	.name 			  = "All font",
	.SupportFontTypeS = ALL_FONT,
	.SupportPT		  =	ALL_SIZE,
	.CodingFormatS	  = FreetypeCodingFormatS,
	.Ops = &FreetypeOps,
};





int FreetypeInit(void)
{
	if(RegisteredFontsChannel(&FontsChannel_songti))
	{
		printf(MODULE_NAME": Registration failed\n");
		return -1;
	}
	return 0;
}

void FreetypeExit(void)
{
	UnregisteredFontsChannel(&FontsChannel_songti);
}



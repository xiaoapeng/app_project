#include <stdio.h>
#include <string.h>
#include <math.h>
#include <wchar.h>
#include <ft2build.h>
#include <freetype/ftglyph.h>

#include "utf8-to-unicode.h"
#include "pixel_display.h"

#include FT_FREETYPE_H


#define WIDTH   480
#define HEIGHT  272
#define FONT_SIZE 24 // 字体大小 单位为点
/* origin is the upper left corner */
unsigned char image[HEIGHT][WIDTH];

typedef struct TGlyph_ { 
	FT_UInt index; /* glyph index 索引	*/ 
	FT_Vector pos; /* glyph origin on the baseline 	笔的位置*/ 
	FT_Glyph image; /* glyph image */ 
} TGlyph, *PGlyph; 



int Get_Glyphs_Frm_Wstr(FT_Face face, wchar_t * wstr, TGlyph glyphs[])
{
	int n;
	PGlyph glyph = glyphs;
	int pen_x = 0;
	int pen_y = 0;
	int error;
	FT_GlyphSlot  slot = face->glyph;;
	
		
	for (n = 0; n < wcslen(wstr); n++)
	{
		//获取索引
		glyph->index = FT_Get_Char_Index( face, wstr[n]); 
		/* store current pen position */ 
		glyph->pos.x = pen_x;
		glyph->pos.y = pen_y;

		/* load时是把glyph放入插槽face->glyph */
		error = FT_Load_Glyph(face, glyph->index, FT_LOAD_DEFAULT);
		if ( error ) 
			continue;

		error = FT_Get_Glyph(face->glyph, &glyph->image ); 
		if ( error ) 
			continue;

		/* translate the glyph image now */ 
		/* 这使得glyph->image里含有位置信息 */
		FT_Glyph_Transform(glyph->image, 0, &glyph->pos );

		pen_x += slot->advance.x;  /* 1/64 point */

		/* increment number of glyphs */ 
		glyph++;		
	}

	/* count number of glyphs loaded */ 
	return (glyph - glyphs);
}
/* Replace this function with something useful. */





void
draw_bitmap( FT_Bitmap*  bitmap,
             FT_Int      x,
             FT_Int      y)
{
  FT_Int  i, j, p, q;
  FT_Int  x_max = x + bitmap->width;
  FT_Int  y_max = y + bitmap->rows;


  for ( i = x, p = 0; i < x_max; i++, p++ )
  {
    for ( j = y, q = 0; j < y_max; j++, q++ )
    {
      if ( i < 0      || j < 0       ||
           i >= WIDTH || j >= HEIGHT )
        continue;

      image[j][i] |= bitmap->buffer[q * bitmap->width + p];
    }
  }
}

//渲染
void Draw_Glyphs(TGlyph glyphs[], FT_UInt num_glyphs, FT_Vector pen)
{
	int n;
	int error;
	
	for (n = 0; n < num_glyphs; n++)
	{
		FT_Glyph_Transform(glyphs[n].image, 0, &pen);
		/* convert glyph image to bitmap (destroy the glyph copy!) */ 
		error = FT_Glyph_To_Bitmap(&glyphs[n].image, FT_RENDER_MODE_NORMAL, 0, /* no additional translation */ 
                              		1 ); 		/* destroy copy in "image" */
		if ( !error ) 
		{ 
			FT_BitmapGlyph bit = (FT_BitmapGlyph)glyphs[n].image; 
			draw_bitmap(&bit->bitmap, bit->left, HEIGHT - bit->top); 
			FT_Done_Glyph(glyphs[n].image ); 
		}
	}
}



void
show_image( void )
{
  int  i, j;

  for ( i = 0; i < HEIGHT; i++ )
  {
    for ( j = 0; j < WIDTH; j++ )
    {
    	if(image[i][j]==0)
    		continue;
    	lcd_put_pixel(j,i,0xffffff);
    }
  }
}

void compute_string_bbox(TGlyph glyphs[], FT_UInt num_glyphs, FT_BBox *abbox )
{
	FT_BBox bbox; 
	int n;
	
	bbox.xMin = bbox.yMin = 32000; 
	bbox.xMax = bbox.yMax = -32000;

	for ( n = 0; n < num_glyphs; n++ )
	{
		FT_BBox glyph_bbox;
		
		FT_Glyph_Get_CBox(glyphs[n].image, FT_GLYPH_BBOX_TRUNCATE, &glyph_bbox );

		if (glyph_bbox.xMin < bbox.xMin)
			bbox.xMin = glyph_bbox.xMin;

		if (glyph_bbox.yMin < bbox.yMin)
			bbox.yMin = glyph_bbox.yMin;

		if (glyph_bbox.xMax > bbox.xMax)
			bbox.xMax = glyph_bbox.xMax;

		if (glyph_bbox.yMax > bbox.yMax)
			bbox.yMax = glyph_bbox.yMax;
	}

	*abbox = bbox;
}






static char* text[]= {
	"000",
	"哈哈哈哈哈",
	"5555555",
	"66666",
	"傻子"

};
#define  TEXT_SIZE  (sizeof(text)/sizeof(text[0]))


int main(int argc,char **argv)
{
	FT_Library	  library;
	FT_Face 	  face;

	FT_GlyphSlot  slot;
	FT_Matrix	  matrix;				  /* 变换矩阵 */ 
	FT_Vector	  pen;					  /* untransformed origin  */
	FT_Error	  error;
	FT_BBox 	  bbox;
	TGlyph 		  glyphs[100];	
	FT_UInt num_glyphs;
	
	char*		  filename;

	double		  angle;
	int 		  target_height;
	int 		  i;

	wchar_t 	  *unicode_str[TEXT_SIZE];
	
	for(i=0;i<TEXT_SIZE;i++)
		unicode_str[i]		= utf8_code_new(text[i],strlen(text[i]));


	
	if ( argc != 2 )
	{
		fprintf ( stderr, "usage: %s font sample-text\n", argv[0] );
		exit( 1 );
	}
	if(pixel_display_init())
		exit(1);
	
	

	
	filename      = argv[1];                           /* 字体文件  */

	
	angle		  = ( 0.0 / 360 ) * 3.14159 * 2;	  /* use 25 degrees 	*/
	target_height = HEIGHT;
	
	error = FT_Init_FreeType( &library );			   /* initialize library */
	/* error handling omitted */

	//得到一个face对象
	error = FT_New_Face( library, filename, 0, &face ); /* create face object */
	/* error handling omitted */
	
	//128x128
	FT_Set_Char_Size(face,FONT_SIZE*64,FONT_SIZE*64,128,128);	//设置字体大小
	slot = face->glyph;

	matrix.xx = (FT_Fixed)( cos( angle ) * 0x10000L );
	matrix.xy = (FT_Fixed)(-sin( angle ) * 0x10000L );
	matrix.yx = (FT_Fixed)( sin( angle ) * 0x10000L );
	matrix.yy = (FT_Fixed)( cos( angle ) * 0x10000L );
	
	
	/* the pen position in 26.6 cartesian space coordinates; */
	/* start at (0,40) relative to the upper left corner  */
	pen.x = 0 * 64;
	pen.y = ( target_height-50 ) * 64;

	// 第一行 第二行
	pen.y=HEIGHT*64;
	for(i=0;i<( sizeof(unicode_str) / sizeof(unicode_str[0]) ) ;i++)
	{
		
		int line_box_width;
		int line_box_height;
		//不渲染求 方框大小
		num_glyphs=Get_Glyphs_Frm_Wstr(face,unicode_str[i],glyphs);
		compute_string_bbox(glyphs, num_glyphs, &bbox);
		line_box_width	= bbox.xMax - bbox.xMin;	//方框宽
		line_box_height = bbox.yMax - bbox.yMin;	//方框长
		//求出笔的位置
		pen.x = (WIDTH - line_box_width)/2 * 64;
		pen.y = (pen.y/64 - line_box_height) * 64;
		//渲染成位图
		Draw_Glyphs(glyphs,num_glyphs,pen);
	}

	//打印在屏幕上
	show_image();
	
	FT_Done_Face	( face );
	FT_Done_FreeType( library );
	pixel_display_exit();
	return 0;
}






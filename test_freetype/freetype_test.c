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
/* origin is the upper left corner */
unsigned char image[HEIGHT][WIDTH];


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


static char* text[]= {
	"嘿嘿嘿",
	"哈哈哈哈哈哈哈哈",
	"zzzzzzzzzzzzz",
	"hhhhhhhhh",

};
#define  TEXT_SIZE  (sizeof(text)/sizeof(text[0]))


int main(int argc,char **argv)
{
	FT_Library	  library;
	FT_Face 	  face;

	FT_GlyphSlot  slot;
	FT_Matrix	  matrix;				  /* transformation matrix */
	FT_Vector	  pen;					  /* untransformed origin  */
	FT_Error	  error;
	FT_BBox 	  bbox;
	FT_Glyph      glyph;
	FT_Pos  	line_box_ymax=0, line_box_ymin =1000;
	
	char*		  filename;

	double		  angle;
	int 		  target_height;
	int 		  n, i;

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
	FT_Set_Char_Size(face,24*64,24*64,128,128);
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
	for(i=0;i<( sizeof(unicode_str) / sizeof(unicode_str[0]) ) ;i++)
	{
		line_box_ymax=0;
		line_box_ymin =1000;
		for ( n = 0; n < wcslen(unicode_str[i]); n++ )
		{
			
			/* set transformation */
			FT_Set_Transform( face, &matrix, &pen );

			/* load glyph image into the slot (erase previous one) */
			printf("code = 0x%08x\n",unicode_str[i][n]);
			error = FT_Load_Char( face, unicode_str[i][n], FT_LOAD_RENDER );
			if ( error )
			 continue;				  /* ignore errors */
			//多行显示代码
			error = FT_Get_Glyph( face->glyph, &glyph );
			if (error)
			{
			printf("FT_Get_Glyph error!\n");
			return -1;
			}
			FT_Glyph_Get_CBox(glyph, FT_GLYPH_BBOX_TRUNCATE, &bbox );
			if (line_box_ymin > bbox.yMin)
			  line_box_ymin = bbox.yMin;
			if (line_box_ymax < bbox.yMax)
			  line_box_ymax = bbox.yMax;
			/* now, draw to our target surface (convert position) */
			draw_bitmap( &slot->bitmap,
					   slot->bitmap_left,
					   target_height - slot->bitmap_top );

			/* increment pen position */
			pen.x += slot->advance.x;
			//pen.y += slot->advance.y;
		}
		printf("i=%d\n",i);
		pen.y = (  pen.y/64 - (line_box_ymax - line_box_ymin) ) * 64;
		pen.x = 0 * 64;
		utf8_code_free(unicode_str[i]);
	}

	
	show_image();
	
	FT_Done_Face	( face );
	FT_Done_FreeType( library );
	pixel_display_exit();
	return 0;
}






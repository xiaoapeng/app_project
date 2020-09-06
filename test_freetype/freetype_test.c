#include <stdio.h>
#include <string.h>
#include <math.h>
#include <wchar.h>
#include <ft2build.h>

#include "utf8-to-unicode.h"


#include FT_FREETYPE_H


#define WIDTH   80
#define HEIGHT  80
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
      putchar( image[i][j] == 0 ? ' '
                                : image[i][j] < 128 ? '+'
                                                    : '*' );
    putchar( '\n' );
  }
}





int main(int argc,char **argv)
{
	FT_Library	  library;
	FT_Face 	  face;

	FT_GlyphSlot  slot;
	FT_Matrix	  matrix;				  /* transformation matrix */
	FT_Vector	  pen;					  /* untransformed origin  */
	FT_Error	  error;

	char*		  filename;
	char*		  text;

	double		  angle;
	int 		  target_height;
	int 		  n, num_chars;

	struct utf8_code	  *utf8_str;
	wchar_t 	  *unicode_str;
	size_t		  unicode_str_len;
	
	if ( argc != 3 )
	{
		fprintf ( stderr, "usage: %s font sample-text\n", argv[0] );
		exit( 1 );
	}

	

	
	filename      = argv[1];                           /* 字体文件  */
	text		  = argv[2];						   /* 字*/
	num_chars	  = strlen( text );
	
	utf8_str 		= utf8_code_new(text,num_chars);
	unicode_str 	= utf8_to_unicode(utf8_str);
	unicode_str_len = wcslen(unicode_str);

	
	angle		  = ( 0.0 / 360 ) * 3.14159 * 2;	  /* use 25 degrees 	*/
	target_height = HEIGHT;
	
	error = FT_Init_FreeType( &library );			   /* initialize library */
	/* error handling omitted */

	//得到一个face对象
	error = FT_New_Face( library, filename, 0, &face ); /* create face object */
	/* error handling omitted */
	
	FT_Set_Pixel_Sizes(face, 24, 0);

	printf("face->num_faces = %d\n",face->num_faces );
	
	slot = face->glyph;

	matrix.xx = (FT_Fixed)( cos( angle ) * 0x10000L );
	matrix.xy = (FT_Fixed)(-sin( angle ) * 0x10000L );
	matrix.yx = (FT_Fixed)( sin( angle ) * 0x10000L );
	matrix.yy = (FT_Fixed)( cos( angle ) * 0x10000L );
	
	
	/* the pen position in 26.6 cartesian space coordinates; */
	/* start at (0,40) relative to the upper left corner  */
	pen.x = 0 * 64;
	pen.y = ( target_height - 40 ) * 64;


	
	for ( n = 0; n < unicode_str_len; n++ )
	{
	  /* set transformation */
	  FT_Set_Transform( face, &matrix, &pen );
	
	  /* load glyph image into the slot (erase previous one) */
	  error = FT_Load_Char( face, unicode_str[n], FT_LOAD_RENDER );
	  if ( error )
		continue;				  /* ignore errors */
	 	
	  /* now, draw to our target surface (convert position) */
	  draw_bitmap( &slot->bitmap,
				   slot->bitmap_left,
				   target_height - slot->bitmap_top );
	
	  /* increment pen position */
	  pen.x += slot->advance.x;
	  pen.y += slot->advance.y;
	}

	
	show_image();
	
	FT_Done_Face	( face );
	FT_Done_FreeType( library );
	
	return 0;
}






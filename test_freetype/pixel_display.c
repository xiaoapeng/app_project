#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


#include <linux/fb.h>
#include "pixel_display.h"





static struct fb_var_screeninfo fb_var;	/* Current var */
static struct fb_fix_screeninfo fb_fix;	/* Current fix */
static int screen_size;
static int fb_fd;

static unsigned char *fbmem;		//显存的地址
static unsigned int line_width;	//x方向的宽度 以字节为单位
static unsigned int pixel_width;	//每一个像素的宽度 以字节为单位 



void lcd_put_pixel(int x, int y, unsigned int color)
{
	unsigned char *pen_8 = fbmem+x*pixel_width+y*line_width;
	unsigned short *pen_16;	
	unsigned int *pen_32;	
	pen_16 = (unsigned short *) pen_8;
	pen_32 = (unsigned int *) pen_8;
	unsigned int rad,green,blue;
	switch (fb_var.bits_per_pixel)
	{
		case 8:
			*pen_8 = (char)color;
			break;
		case 16:
			rad = (color >> 16) & 0xff;
			green = (color >> 8) & 0xff;
			blue = (color >> 0) & 0xff;
			*pen_16 = ((rad>>3)<<(5+6))|((green>>2)<<5) | ( blue >> 3);
			break;
		case 32:
			*pen_32 = (char)color;
			break;
		default:
			printf("can't surport  %dbpp ",fb_var.bits_per_pixel);
			break;
	}
}



int pixel_display_init(void)
{
	fb_fd=open("/dev/fb0",O_RDWR);
	if( fb_fd < 0 )
	{
		perror("can't open /dev/fb0\n/dev/fb0:");
		return -1;
	}
	if( ioctl(fb_fd,FBIOGET_VSCREENINFO,&fb_var) )
	{
		perror("can't get var\n/dev/fb0 ioctl:");
		return -1;
	}
	if( ioctl(fb_fd,FBIOGET_FSCREENINFO,&fb_fix) )
	{
		perror("can't get var\n/dev/fb0 ioctl:");
		return -1;
	}
	line_width = fb_var.xres * fb_var.bits_per_pixel / 8;
	pixel_width = fb_var.bits_per_pixel / 8;
	screen_size = fb_var.xres * fb_var.yres * fb_var.bits_per_pixel / 8;
	fbmem = (unsigned char *)mmap(NULL , screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, 0);
	if(fbmem == (unsigned char*)-1)
	{
		perror("can't mmap fbmem\n/dev/fb0 mmap");
		return -1;
	}	
	memset(fbmem, 0, screen_size);
	return 0;
}


void pixel_display_exit(void)
{
	munmap(fbmem, screen_size);
	close(fb_fd);
}













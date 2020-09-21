#ifndef __TEXT__CTRL__H
#define __TEXT__CTRL__H


/* 
 *	TextShow函数参数iFalg 取值
 *
 *	SHOW_NEXT_PAGE			显示下一页
 *	SHOW_PREV_PAGE			显示上一页
 *	SHOW_CURRENT_PAGE		显示当前页
 *
 *
 */
#define SHOW_NEXT_PAGE		0x0
#define	SHOW_PREV_PAGE		0x1
#define	SHOW_CURRENT_PAGE 	0x2



/*
 *	TextShow函数返回值
 *	TEXT_SUCCEED		成功
 *	TEXT_NO_NEXT		没有下一页了
 *	TEXT_NO_PREV		没有上一页
 *	TEXT_NO_CURRENT		没有当前页	这种情况是文件长度为0的情况
 *	TEXT_ERROR			发生错误
 */
#define TEXT_SUCCEED 			0
#define TEXT_NO_NEXT 			0x1
#define TEXT_NO_PREV			0x2
#define TEXT_NO_CURRENT			0x3
#define TEXT_ARISE_ERROR		0x4

/* 
 *	控制字符映射结构体
 * 	code_num	映射后的编码格式
 *	target_char	映射后的编码值
 *	ascii_char	映射前的ascii
 */
struct ctrl_char{
	wchar_t target_char;
	char 	ascii_char;
};
#define StructureEntry(target_char,ascii_char)	{ (target_char), (ascii_char) }





/********************************
 *	页描述符结构体
 *		desc 		显示描述符
 *		read		读指针 在文件中的偏移值
 *		len			该页的长度
 *		is_ready	就绪标志
 ********************************/
struct page_desc{
	int desc;
	int read;
	int len;
	int is_ready;
};



/****************************************
 *	控制器结构体
 *
 *	file_desc 				文件描述符

 *
 *	coding_format			编码格式
 *	a_ctrl_char				控制字符映射表
 *	file_size				文件大小
 *	file_read				文件读写指针 即将要读的字符数
 *	file_buf				文件缓冲区，避免反复读，提高效率
 *	file_buf_read			缓冲区的第一个字符对应文件中的位置
 ****************************************/
 #define CTRL_CHAR_COUNT	0x21
 #define CODEING_STR_SIZE  20
 #define FILE_BUF_SIZE		4096
 struct text_ctrl{
	int file_desc;

	struct page_desc next;	
	struct page_desc prev;
	struct page_desc current;
	
	struct ctrl_char  *p_ctrl_char;
	int file_size;
	char file_buf[FILE_BUF_SIZE];
	int file_buf_read;
} ;
typedef struct text_ctrl TEXT_CTRL;


/*	用户设置使用text_ctrl的结构体
 *		word_spacing		字距
 *		line_spacing		行距
 *		word_size			字号
 *		direction			方向
 *		font				字体
 *		coding_format		编码格式
 *		filename			要打开的文件名
 */
struct text_set{
	int 		word_spacing;
	int 		line_spacing;
	int 		word_size;
	int 		direction;
	int			coding_format;
	char*		font;	
	char*		filename;
};
typedef struct text_set TEXT_SET;



#endif /* __TEXT__CTRL__H */


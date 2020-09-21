
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wchar.h>
#include <string.h>

#include <encoding_manager.h>
#include <text_display.h>

#define FB_NAME "fb-dev"


int Init(void)
{
	int error;
	error=CodeInit();
	if(error)
		return -1;
	error = TextDisplayInit();
	if(error)
		return -1;
	return 0;
}
void Exit(void)
{
	CodeExit();
	TextDisplayExit();
}

char Text[] = "\t\t\t深度深度是实打实的第三方京东方仍坚持那么凉"\
			  "快几点睡佛is饿gy哦a发顺s丰\n\n\n\n列g数\n\n\n\n\n\n我觉得开挖低价位"\
			  "近段时间的接建了坚实的卡是简单地的深度看来是sdsd傻是滴是滴死哦"; 
int main(void)
{
	struct text_formatting  info;
	int text_desc;
	wchar_t *target_code;
	int i,error;
	if(Init())
		return -1;
	/* 申请一个文本显示描述符 */
	info.CodingFormat = "utf16-le";
	info.direction = VERTICAL;
	info.font		= "fzktjw";
	info.space_code = ' ';		
	info.line_spacing	= 5;
	info.word_size	= 10;
	info.word_spacing	= 4;
	text_desc = TextDispNew(&info);
	if(text_desc)
	{
		return -1;
	}

	/* 进行编码的转化 */
	target_code = CodeConversion(CODE_UTF8, CODE_UTF16_LE, Text, strlen(Text), NULL);
	if(target_code == NULL)
		return -1;
	for(i=0;i<wcslen(target_code);i++)
	{
		if(target_code[i] <= 0x1f)
		{
			error = TextAddCtrlWord(text_desc, (char)target_code[i]);
		}
		else
			error = TextAddWord(text_desc, target_code[i]);
			
		if(error == TEXT_EOF)
		{
			printf("TextAddWord :TEXT_EOF\n");
			break;
		}
		if(error == TEXT_ERROR)
		{
			printf("TextAddWord :TEXT_ERROR\n");
			return -1;
		}
	}
	CodeFreeCodeData(target_code);
	TextDisplay(text_desc);
	Exit();
	return 0;
}





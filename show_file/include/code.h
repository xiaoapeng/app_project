#ifndef __CODE_H___
#define __CODE_H___ 1

#include <string.h>

#define CODE_GB2312 		0
#define CODE_UTF8			1
#define CODE_UTF16_LE		2
#define CODE_UTF16_BE		3

#define CodeingFormatSize	64
extern char *CodeingFormat[CodeingFormatSize];

#define SetCodeingFormat(code_num,str)	CodeingFormat[(code_num)] = (str)
#define GetCodeingFormatStr(code_num)			CodeingFormat[(code_num)]

static inline int GetCodeingFormatNum(const char *strCode)
{
	int i;
	if((unsigned int)Code >=CodeingFormatSize)
	{
		return NULL;
	}
	for(i=0;i<CodeingFormatSize;i++)
	{
		if(strcmp(GetCodeingFormatStr(i),strCode ) == 0)
			return i;
	}
	return -1;
}

#endif 


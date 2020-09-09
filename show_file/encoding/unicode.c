/*****************************************
 *将其他编码转化为unicode
 *支持 utf-8
 *
 *
 *
 *****************************************/

#include <wchar.h>
#include <stdio.h>
#include <string.h>
#include <encoding_manager.h>


#ifndef ARRY_SIZE
# define ARRY_SIZE(A) (sizeof(A)/sizeof(A[0]))
#endif

#define MODULE_NAME "unicode"


/* 计算有多少个UTF8字符 */
static int utf8_wrod_count(unsigned const char *_utf8,size_t len)
{
	size_t i;
	int count=0;
	unsigned const char *utf8;
	utf8 = (const unsigned char*) _utf8 ;
	unsigned char var;
	for(i=0;i<len;i++)
	{	
		var = (unsigned char)utf8[i];
		if( var <= 0x7F ){	//ascii码
			count++;
			continue;
		}
		//先解决乱码情况 10xxxxxx 		   11111xxx开头就是乱码情况
		if( (( var >> 6 ) == 0x2 ) || ( var >> 3) == 0x1F)
			return 0;
			
		//正常utf8码
		var = utf8[i++]<<1;
		while( (var & (1<<7)) )
		{
			if( i<len && (utf8[i] >> 6 == 0x2 && i) )
			{
				i++;
				var = var << 1;
			}
			else
				return 0;
		}
		i--;
		if( !(var & (1<<7)) )
			count++;
		else
			return 0;
	}
	return count;

}



static wchar_t * utf8_to_unicode_le(unsigned const char *strSrcCode, unsigned long ulCodeLen)
{
	int icount,j=0;;
	unsigned long text =0, tmp, i, text_i  ;
	unsigned char var;
	wchar_t *ptCodeDateStr;
	if( strlen((const char *)strSrcCode) < ulCodeLen )
	{
		printf(MODULE_NAME": The true length is less than the incoming length\n(len = %lu)\n", 
										ulCodeLen);
		return NULL;
	}
	/* 计算编码转化后的长度 在申请内存时+1*/
	icount = utf8_wrod_count(strSrcCode,(size_t)ulCodeLen);
	if(!icount)
		return NULL;
	ptCodeDateStr = CodeAllocCodeData( (icount+1)*sizeof(wchar_t),CODE_UTF16_LE);
	
	for(i=0;i<ulCodeLen;i++)
	{	
		var = (unsigned char)strSrcCode[i];
		if( var <= 0x7F ){	//ascii码
			ptCodeDateStr[j++] = var;
			continue;
		}
		//正常utf8码
		var = strSrcCode[i++]<<1;
		text = 0;
		for(text_i=0;var & (1<<7);i++,text_i++)
		{
			tmp = strSrcCode[i];
			tmp &= ~(3<<6);
			text = text << (text_i*6);
			text |= tmp;
			var = var << 1;
		}
		var = var >> (text_i+1);
		tmp = var;
		text |= tmp <<(text_i*6);
		ptCodeDateStr[j++] = text;
		i--;
	}
	return ptCodeDateStr;
	
}


/********************************************************
 * 编码转化函数
 ********************************************************/
static wchar_t * UnicodeLECodeGoalConversion(unsigned long ulSrcCodeFormat, 
				unsigned const char *strSrcCode,unsigned long ulCodeLen)	
{
	wchar_t *pdwCodeTarget;
	switch (ulSrcCodeFormat)
	{
		case CODE_UTF8 :
			pdwCodeTarget = utf8_to_unicode_le(strSrcCode,ulCodeLen);
			return pdwCodeTarget ;
			break;
		default :
			printf(MODULE_NAME": This code is not supported (Does not support ID:%lu)\n",
						ulSrcCodeFormat);
			return NULL;
	}


}


/********************************************************
 * 编码识别函数
 ********************************************************/
static int ThisCodeIdentify(unsigned const char *strSrcCode, 
									unsigned long ulCodeLen)
{	
	int err;
	err=utf8_wrod_count(strSrcCode, (size_t)ulCodeLen);
	if(err>0)
		return CODE_UTF8;
	/* 后续识别代码添加在下面 */
	return -1;
}



/********************************************************
 * 释放转化编码时所产生的数据
 ********************************************************/
void  ThisTargetCodeFree(wchar_t *pdwStr)
{
	CodeFreeCodeData(pdwStr);
}




static unsigned long uaUnicodeLESupportID[] = {
	CODE_UTF8,
};

static struct CodeOpr tUnicodeLEOpr ={
	.CodeGoalConversion = UnicodeLECodeGoalConversion,
	.CodeIdentify = ThisCodeIdentify,
	.TargetCodeFree = ThisTargetCodeFree,
};



static struct CodeModule  UnicodeLEModule = {
	.name = "utf16-le",
	.ulID = CODE_UTF16_LE,
	.puSupportID = uaUnicodeLESupportID,
	.uNum = ARRY_SIZE(uaUnicodeLESupportID),
	.pt_opr	=	&tUnicodeLEOpr,
};

/********************************************************
 * 模块初始化函数
 ********************************************************/

int UnicodeModuleInit(void)
{
	if(RegisterCodeModule(&UnicodeLEModule)<0)
	{
		printf(MODULE_NAME": Registration failed\n (name = %s , ID = %lu)\n",
						UnicodeLEModule.name,UnicodeLEModule.ulID);
		return -1;
	}
	return 0;

}

/********************************************************
 * 模块退出函数
 ********************************************************/

void UnicodeModuleExit(void)
{
	UnregisterCodeModule(&UnicodeLEModule);
}


#include <string.h>
#include <wchar.h>
#include <stdlib.h>
#include <stdio.h>
#include "utf8-to-unicode.h"





/*计算有多少个utf8字符*/
static int utf8_wrod_count(const char *_utf8,size_t len)
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

/*	构造一个 utf8 对象
 *	@str = "utf8编码"
 *	@len = ""
 *
 */
struct utf8_code*  utf8_code_new(const char* str,size_t len)
{
	struct utf8_code * utf8_codep;
	int count ;
	if( strlen(str) < len )
		return NULL;
	count = utf8_wrod_count(str,len);
	if(count == 0)
		return NULL;
	utf8_codep = (struct utf8_code *)malloc(sizeof(struct utf8_code));
	if(utf8_codep == NULL)
		return NULL;
	memset(utf8_codep, 0, sizeof(struct utf8_code));
	utf8_codep->utf8_str = str;
	utf8_codep->utf8_len = len;
	utf8_codep->target_code = (wchar_t *) malloc( (count+1) * sizeof(wchar_t) );
	if(utf8_codep->target_code==NULL)
	{
		free(utf8_codep);
		return NULL;
	}
	memset(utf8_codep->target_code,0,count+1);
	utf8_codep->target_len = count;
	return utf8_codep;
}
void  utf8_code_free(struct utf8_code* utf8_codep)
{
	if(utf8_codep == NULL)
		return;
	free(utf8_codep->target_code);
	free(utf8_codep);
}



/*	
 * 编码转化函数
 */
wchar_t * utf8_to_unicode(struct utf8_code* utf8_codep)
{
	wchar_t * target_code = utf8_codep->target_code;
	unsigned char *utf8_str = (unsigned char *) utf8_codep->utf8_str;
	size_t utf8_len = utf8_codep->utf8_len;
	size_t i,text_i;
	unsigned long text =0 ,tmp;
	unsigned char var;
	int j=0;
	if(utf8_codep ==NULL)
		return NULL;
	for(i=0;i<utf8_len;i++)
	{	
		var = (unsigned char)utf8_str[i];
		if( var <= 0x7F ){	//ascii码
			target_code[j++] = var;
			continue;
		}
		//正常utf8码
		var = utf8_str[i++]<<1;
		text = 0;
		for(text_i=0;var & (1<<7);i++,text_i++)
		{
			tmp = utf8_str[i];
			tmp &= ~(3<<6);
			text = text << (text_i*6);
			text |= tmp;
			var = var << 1;
		}
		var = var >> (text_i+1);
		tmp = var;
		text |= tmp <<(text_i*6);
		target_code[j++] = text;
		i--;
	}
	return target_code;
}
#if 0
int main(void)
{
	char str[] = "x小明呀sdsd下单数的dsdsdsdssd哈哈颠三倒四多所多受持读诵的";
	struct utf8_code* utf8_p;
	unsigned long i;
	wchar_t *wcharp;
	utf8_p =utf8_code_new(str,strlen(str));
	if(utf8_p == NULL)
	{
		printf("utf8_code_new: error\n");
		return -1;
	}
	wcharp=utf8_to_unicode(utf8_p);
	for(i=0;wcharp[i];i++)
	{
		printf("wcharp[%lu]=0x%04x\n",i,wcharp[i]);
	}
	utf8_code_free(utf8_p);
	return 0;
}
#endif



#ifndef __UTF8_TO_UNICODE_H__
#define __UTF8_TO_UNICODE_H__    1



struct utf8_code{
	const char *utf8_str;
	size_t utf8_len;
	size_t target_len;
	wchar_t target_code[0];

};

extern wchar_t *  utf8_code_new(const char* str,size_t len);

extern void  utf8_code_free(wchar_t *s);




#endif



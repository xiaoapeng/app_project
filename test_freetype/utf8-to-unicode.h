#ifndef __UTF8_TO_UNICODE_H__
#define __UTF8_TO_UNICODE_H__    1



struct utf8_code{
	const char *utf8_str;
	size_t utf8_len;
	wchar_t *target_code;
	size_t target_len;

};

extern struct utf8_code*  utf8_code_new(const char* str,size_t len);
extern void  utf8_code_free(struct utf8_code* utf8_codep);
extern wchar_t * utf8_to_unicode(struct utf8_code* utf8_codep);




#endif



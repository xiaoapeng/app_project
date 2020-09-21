/*
 *	本文件提供给text_ctrl使用
 *	应用场景,当数据量比较大时使用该文件
 *	本文件提供一个无限大的栈，当然在磁盘
 *	充足的情况下
 */

#include <stdio.h>



/* 命名计数，防止在多个文件使用时文件名冲突 */
static unsigned name_count=0;
#define FILE_NAME	"file_stack"
#define MODULE_NAME "text_stack"

#define FILE_NAME_SIZE 	40
#define BUF_SIZE		1024

/*
 *	file_stack 对象结构
 *	filename	用于缓冲的文件名
 *	file_desc	文件描述符
 *	top			栈顶指针
 *	buf			缓冲区
 */
struct file_stack {
	char filename[FILE_NAME_SIZE];
	int file_desc;
	unsigned long top;
	char buf[BUF_SIZE];
};




/* 
 *	申请一个新的栈对象
 *	返回一个栈对象
 */
struct file_stack* FileStackNew(void)
{
	struct file_stack* ptFileStack;
	int file_desc;
	ptFileStack = (struct file_stack*)malloc(sizeof(struct file_stack));
	if(ptFileStack == NULL)
	{
		printf(MODULE_NAME": Space allocation failure\n");
		return NULL;
	}
	/* 
	 *	希望这里不要溢出,不过即使数组溢出一般也不影响程序，
	 *	后面还有1024的缓冲区 
	 */
	sprintf(ptFileStack->filename,FILE_NAME"%ul.stack",name_count);
	ptFileStack->filename[FILE_NAME_SIZE-1] = '\0';
	file_desc = open(ptFileStack->filename,O_RDWR|O_CREAT|O_EXCL)
	if(file_desc < 0)
	{
		printf(MODULE_NAME": File creation failed\n");
		return NULL;
	}
	ptFileStack->file_desc=file_desc;
	ptFileStack->top = 0;
	return ptFileStack;
}

int FilePush() 








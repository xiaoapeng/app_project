#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <wchar.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>




#include <encoding_manager.h>
#include <text_display.h>
#include <text_ctrl.h>

#define MODULE_NAME "text_ctrl" 


/*
 *	编码控制字符映射表
 *	coding_num		对应的编码
 *	coding_map		ascii与该编码的映射
 */
struct coding_ctrl_char{
	int coding_num;
	struct ctrl_char *coding_map;
	wchar_t space_char;
};

/* 支持的编码 	 如果是其他编码       	 就需要转化 */
static int g_SupportedEncoding[]={CODE_UTF16_LE,CODE_GB2312};
#define SUPPORTEENCODING_SIZE sizeof(g_SupportedEncoding)

/* utf16le 与gb2312对控制字符的映射是一样的 */
static struct ctrl_char utf16le_Gb2312[] ={
	StructureEntry(0x01, 0x01),StructureEntry(0x02, 0x02),StructureEntry(0x03, 0x03),
	StructureEntry(0x04, 0x04),StructureEntry(0x05, 0x05),StructureEntry(0x06, 0x06),
	StructureEntry(0x07, 0x07),StructureEntry(0x08, 0x08),StructureEntry(0x09, 0x09),
	StructureEntry(0x0A, 0x0A),StructureEntry(0x0B, 0x0B),StructureEntry(0x0C, 0x0C),
	StructureEntry(0x0D, 0x0D),StructureEntry(0x0E, 0x0E),StructureEntry(0x0F, 0x0F),
	StructureEntry(0x10, 0x10),StructureEntry(0x11, 0x11),StructureEntry(0x12, 0x12),
	StructureEntry(0x13, 0x13),StructureEntry(0x14, 0x14),StructureEntry(0x15, 0x15),
	StructureEntry(0x16, 0x16),StructureEntry(0x17, 0x17),StructureEntry(0x18, 0x18),
	StructureEntry(0x19, 0x19),StructureEntry(0x1A, 0x1A),StructureEntry(0x1B, 0x1B),
	StructureEntry(0x1C, 0x1C),StructureEntry(0x1D, 0x1D),StructureEntry(0x1E, 0x1E),
	StructureEntry(0x1F, 0x1F),StructureEntry(0x7F, 0x7F),StructureEntry(0x00, 0x00),
};

static struct coding_ctrl_char  a_coding_ctrl_char[] = {
	{CODE_UTF16_LE,utf16le_Gb2312,' '},
	{CODE_GB2312,utf16le_Gb2312,' '},
	{-1,NULL,'\0'},
};

static struct coding_ctrl_char * look_for_ctrl_char(int coding)
{
	struct coding_ctrl_char * pos;
	pos = a_coding_ctrl_char;
	while(pos->coding_num != -1)
	{
		if(pos->coding_num == coding)
			return pos;
		pos++;
	}
	return NULL;
}
static char GetCtrlChar(wchar_t Coding,struct ctrl_char* p_ctrl_char)
{
	struct ctrl_char* pos;
	pos = p_ctrl_char;
	while(pos->ascii_char != 0x00 )
	{
		if(pos->target_char == Coding )
			return pos->ascii_char;
		pos ++ ;
	}

	return -1;
}

#define READ_END 0
#define WRITE_END 1

#define BUF_SIZE 4096

static void _signal_handler(int signo)
{
	/* 给子进程收尸 */
	if (signo == SIGCHLD) {
        while (waitpid(-1, NULL, WNOHANG) > 0) 
        {
        	/* NULL */	
        }
    }
}


/* 建立子进程进行编码转化 */
static int Conversion(struct text_set* ptTextSet,int TargetCodeNum)
{
	pid_t ConversionPid;
	int fd[2];
	int error;
	int var;
	int SrcCodeNum;
	char filename;
	char new_filename[24];
	int i;
	/* 做好准备工作 */
	SrcCodeNum = ptTextSet->coding_format;

	filename = ptTextSet->filename;
	if(filename == NULL)
		return -1;
	if(strlen(filename)>(24-1-4))
	{
		/* 大于必须截断 */
		for(i=0;i<(24-1-4))
			new_filename[i] = filename[i];
		new_filename[i] = '\0';
		strcat(new_filename,".tem");
	}
	else
	{
		strcpy(new_filename, filename);
		strcat(new_filename,".tem");
	}


	
	/* 先建立通信，使用无名管道 */
	error = pipe(fb);
	if(error)
	{
		printf(MODULE_NAME": Unable to create nameless pipe\n");
		return -1;
	}
	error = fork();
	if(error < 0)
	{
		printf(MODULE_NAME": Child process creation failed\n");
		return -1;
	}
	if(error == 0)
	{
		int src_fd,target_fd;
		int err;
		char * buf,*read_buf;
		struct stat src_stat;
		wchar_t *target_coding;
		int read_count,Conversion_len, residue_len,write_len;
		/* 子进程 */
		/* 关闭读描述符 */
		close(fd[READ_END]);
		/* 打开源编码文件 */
		src_fd = open(filename,O_RDONLY);
		if(src_fd < 0)
		{
			err = -1;
			printf(MODULE_NAME": Failed to open file\n");
			goto src_open;
		}
		/* 进行mmap映射 */
		err = stat(filename, &src_stat)
		if(err<0)
		{
			err = -1;
			printf(MODULE_NAME": Failed to open file\n");
			goto src_stat;
		}
		buf =(char*) mmap(NULL,src_stat.st_size ,PROT_READ ,MAP_SHARED,src_fd,0);
		if(buf = (char*)-1)
		{
			err = -1;
			printf(MODULE_NAME": Failed to open file\n");
			goto src_mmap;
		}
		read_buf = buf;
		/* 创建目标编码文件 */
		target_fd = open(new_filename, O_WRONLY|O_CREAT|O_TRUNC)
		if(target_fd<0)
		{	
			err = -1;
			printf(MODULE_NAME": File creation failed\n");
			goto target_open;
		}
		residue_len = (int)src_stat.st_size;
		while(residue_len)
		{
			
			Conversion_len = BUF_SIZE < residue_len? BUF_SIZE : residue_len;
			target_coding = CodeConversion(SrcCodeNum, TargetCodeNum,read_buf,Conversion_len, &read_count);
			if(target_coding == NULL)
			{
				err = -1;
				printf(MODULE_NAME": Convert defeat\n");
				goto target_CodeConversion;
			}
			/* 写入进新文件中去 */
			write_len = wcslen(target_coding)*sizeof(wchar_t);

			err = write(target_fd,target_coding,write_len);
			CodeFreeCodeData(target_coding);
			if(err != write_len)
			{
				err = -1;
				printf(MODULE_NAME": write error\n");
				goto target_write:
			}
			/* 进行递增 */
			read_buf += read_count;
			residue_len -= read_count;
			
			if(fd[WRITE_END] != -1 && (read_buf-buf) >= (BUF_SIZE*8))
			{
				err = 0;
				write(fd[WRITE_END],&err,sizeof(int));
				close(fd[WRITE_END]);
				fd[WRITE_END] = -1;
			}
			
		}
		printf(MODULE_NAME": Conversion succeed\n");
		
		target_write:
		target_CodeConversion:
		close(target_fd);
		target_open:
		munmap(buf,src_stat.st_size);
		src_mmap:
		src_stat:
		close(src_fd);
		src_open:
		if(fd[WRITE_END] != -1)
		{
			write(fd[WRITE_END],&err,sizeof(int));
			close(fd[WRITE_END]);
		}
		exit(0);
	}
	else
	{
		/* 父进程 */
		/* 关闭写描述符 */
		signal(SIGCHLD,_signal_handler);
		close(fd[WRITE_END]);
		read(fd[READ_END],&var,sizeof(int));
		close(fd[READ_END]);
		/* 通过判断var来判断子进程的操作是否成功 */
		if(var < 0)
			return -1;
		ptTextSet->coding_format = TargetCodeNum;
		return open(new_filename,O_RDONLY);
	}
}

#undef BUF_SIZE
#undef WRITE_END
#undef READ_END



/* 进行编码处理 		返回文件描述符 */
static int CodingProcessor(struct text_set* ptTextSet)
{
	int SrcCodeNum,TargetCodeNum = -1;
	int i;
	SrcCodeNum = ptTextSet->coding_format;
	switch (SrcCodeNum)
	{
		case CODE_UTF16_LE:
		case CODE_GB2312:
			return open(ptTextSet->filename,O_RDONLY);
		break;

		default :
		/* 需要进行编码转化 当然要进行枚举*/
		for(i=0;i<SUPPORTEENCODING_SIZE;i++)
		{
			if(CodeConversionTest(SrcCodeNum,g_SupportedEncoding[i]))
				TargetCodeNum = g_SupportedEncoding[i];
		}
		break;
	}
	if(TargetCodeNum == -1)
		return -1;
	/* 在下面进行编码转化 		  可能会使用几毫秒的时间 */
	return Conversion(ptTextSet,TargetCodeNum);
}


/**************************************************
 *	打开一个文本文件
 *	参数：
 *		ptTextSet	其他接口需要构造的结构 详情请看结构体成员
 *	返回值：
 *		返回一个控制对象
 *
 **************************************************/
struct text_ctrl* TextOpen(struct text_set* ptTextSet)
{
	struct text_ctrl* ptTextCtrl;
	struct coding_ctrl_char * pt_coding_ctrl_char;
	struct text_formatting  Info;
	struct stat file_info;
	wchar_t *SpaceCode;
	wchar_t *CtrlWchar;
	int i,iDesc;
	int CodeNum;
	/* 进行必要的编码处理 得到文件描述符*/
	iDesc = CodingProcessor(ptTextSet);
	if( iDesc == -1)
	{
		printf(MODULE_NAME": The encoding format is not supported\n");
		return NULL;
	}
	
	CodeNum = ptTextSet->coding_format;
	pt_coding_ctrl_char = look_for_ctrl_char(CodeNum);

		
	/* 动态分配一个对象 */
	ptTextCtrl = (struct text_ctrl*) malloc(sizeof(struct text_ctrl));
	if(ptTextCtrl == NULL)
	{
		printf(MODULE_NAME": Space allocation failure\n");
		return NULL;
	}
	memset(ptTextCtrl,0,sizeof(struct text_ctrl));

	/* 1.设置Info，获取页描述符 */
	Info.CodingFormat = ptTextSet->coding_format;
	Info.direction = ptTextSet->direction;
	Info.font		= ptTextSet->font;
	Info.space_code = pt_coding_ctrl_char->space_char;
	Info.line_spacing	= ptTextSet->line_spacing;
	Info.word_size	= ptTextSet->word_size;
	Info.word_spacing	= ptTextSet->word_spacing;
	/* 2.设置TextCtrl对象 */
	/* 2.1获取当前页 和下一页的描述符 和上一页描述符*/
	ptTextCtrl->current.desc = TextDispNew(&Info);
	if(ptTextCtrl->current.desc == -1)
	{
		return NULL;
	}
	ptTextCtrl->prev.desc = TextDispNew(&Info);
	if(ptTextCtrl->prev.desc == -1)
	{
		return NULL ;		
	}
	ptTextCtrl->next.desc = TextDispNew(&Info);
	if(ptTextCtrl->next.desc == -1)
	{
		return NULL;
	}
	/* 2.2获取文件关键信息 */
	ptTextCtrl->file_desc = iDesc;
	
	if(stat(ptTextSet->filename,&file_info) == -1)
	{	
		printf(MODULE_NAME": %s:Failed to get file status",ptTextSet->filename);
		return NULL;
	}
	ptTextCtrl->file_size =	(int)file_info.st_size;
	
	
	/* 生成控制字符列表 */
	
	ptTextCtrl->p_ctrl_char = pt_coding_ctrl_char->coding_map;
	/* 就绪当前页，和下一页 */
	
	
	
	return ptTextCtrl;
}



enum e_text_state{
	
};

struct text_state{
	wchar_t target_coding;
	int state;
};

wchar_t GetCoding(struct text_ctrl* ptTextCtrl)
{
	

}

#define TEXT_NEXT 		0x0
#define TEXT_PREV 		0x1
#define TEXT_CURRENT 	0x2
/* 
 *	对页进行就绪
 *	参数：
 *		ptTextCtrl		对象指针
 *		iFlag			需要对哪一页进行就绪
 *			TEXT_NEXT		下一页
 *			TEXT_PREV		上一页
 *			TEXT_CURRENT	当前页
 *	返回值：
 *		0 成功
 *		-1 失败
 *		
 *
 */
 #define RETURN_TEXT_SUCCEED 0
 #define RETURN_TEXT_EOF 	-1
 #define RETURN_TEXT_ERROR	-2
 
static	int TextReady(struct text_ctrl* ptTextCtrl,int iFlag)
{
	wchar_t Coding;
	struct page_desc *ptPageDesc;

	int error;
	char ctrl_char;
	switch (iFlag)
	{
		case TEXT_CURRENT:
			/* 一般情况下只会在open中调用到 */
			ptPageDesc = &ptTextCtrl->current;
		break;
		case TEXT_NEXT:
			ptPageDesc = &ptTextCtrl->next;
			/*  */
		break;
		case TEXT_PREV:
			ptPageDesc = &ptTextCtrl->prev;
			/* 由于要获取上一页 		需要调整读指针的位置 */
			error = AdjustmentToPrev(ptTextCtrl);
			if(error)
				return error
		break;
	}
	while(1)
	{
		Coding = GetCoding(ptTextCtrl);
		if(Coding == 0 && ptPageDesc->is_ready == 0)
		{
			return RETURN_TEXT_EOF;
		}
		/* 进行编码判断，查看是控制字符还是可打印的普通字符 */
		ctrl_char = GetCtrlChar(Coding, ptTextCtrl->p_ctrl_char);
		if(ctrl_char != -1)
		{
			/* 控制字符 */
			error =  TextAddCtrlWord(ptPageDesc->desc, ctrl_char);
		}
		else
		{
			/* 普通字符 */
			error = TextAddWord(ptPageDesc->desc,Coding);
		}

		if(error == 0)
		{
			ptPageDesc->is_ready = 1;
			continue;
		}
		if(error == TEXT_EOF)
			break;
		if(error == TEXT_ERROR)
		{
			printf(MODULE_NAME": Error adding character\n");
			return RETURN_TEXT_ERROR;
		}		
	}
	return TEXT_CURRENT;
	
}



int TextShow(struct text_ctrl* ptTextCtrl,int iFlag)
{
	struct page_desc PageDescTmp;	
	switch (iFlag)
	{
		case SHOW_NEXT_PAGE:
			/* 先查看下一页是否就绪 */
			if(!ptTextCtrl->next.is_ready)
			{
				/*	没有就绪就说明没有下一页 */
				return TEXT_NO_NEXT;
			}
			/* 将下一页设置为当前页				    */
			/* 将当前页设置为上一页 			 */
			PageDescTmp = ptTextCtrl->prev;
			ptTextCtrl->prev = ptTextCtrl->current;
			ptTextCtrl->current = ptTextCtrl->next;
			ptTextCtrl->next = PageDescTmp;
			/* 显示设置后的当前页 */
			if(TextDisplay(ptTextCtrl->current.desc) == -1)
				return TEXT_ARISE_ERROR;

			/* 重新配置下次显示的下一页，提高操作效率 */
			
			/*
			 *	由上面的代码可以看出 此时的 ptTextCtrl->next 是即将准备丢弃的prev
			 *	在这个内容丢弃之前，我们要先将他们压在栈中,当我们往回翻页的时候，
			 *	再将他们弹出来
			 */
			
			
			TextClean(ptTextCtrl->next.desc);
			/* 设置下一页未就绪 */
			ptTextCtrl->next.is_ready = 0;
			if(TextReady(ptTextCtrl,TEXT_NEXT) == RETURN_TEXT_ERROR)
				return TEXT_ARISE_ERROR;
		break;

		case SHOW_CURRENT_PAGE:
			if(!ptTextCtrl->current.is_ready)
				return TEXT_NO_CURRENT;
			/* 直接将当前页在屏幕上描绘 */			
			if(TextDisplay(ptTextCtrl->current.desc) == -1)
				return TEXT_ARISE_ERROR;
		break;

		case SHOW_PREV_PAGE:
			if(!ptTextCtrl->prev.is_ready)
				return TEXT_NO_PREV;
			/* 将上一页设置为当前页          */
			/* 将当前页设置为下一页 */
			PageDescTmp = ptTextCtrl->next;
			ptTextCtrl->next = ptTextCtrl->current;
			ptTextCtrl->current = ptTextCtrl->prev;
			ptTextCtrl->prev = PageDescTmp;

			/* 显示配置后的当前页 */
			if(TextDisplay(ptTextCtrl->current.desc) == -1)
				return TEXT_ARISE_ERROR;
			/* 重新配置下次显示        新的上一页 */
			TextClean(ptTextCtrl->prev.desc);
			/* 设置上一页未就绪 */
			ptTextCtrl->prev.is_ready = 0;
			if(TextReady(ptTextCtrl,TEXT_PREV) == RETURN_TEXT_ERROR)
				return TEXT_ARISE_ERROR;
			
		break;

		default:
			return TEXT_ARISE_ERROR;
		break;
	}
}








/* 初始化函数 */
init TextCtrlInit(void)
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

/* 结束退出函数 */
void TextCtrlExit(void)
{
	CodeExit();
	TextDisplayExit();
}







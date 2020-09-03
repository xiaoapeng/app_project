CROSS_COMPILE  =  
AS  =  $(CROSS_COMPILE)as
LD  =  $(CROSS_COMPILE)ld
CC  =  $(CROSS_COMPILE)gcc
CPP  =  $(CC)  -E 
AR  =  $(CROSS_COMPILE)ar 
NM  =  $(CROSS_COMPILE)nm 
STRIP  =  $(CROSS_COMPILE)strip 
OBJCOPY  =  $(CROSS_COMPILE)objcopy 
OBJDUMP  =  $(CROSS_COMPILE)objdump 


export  AS  LD  CC  CPP  AR  NM 
export  STRIP  OBJCOPY  OBJDUMP 

CFLAGS  :=  -Wall  -O2  -g    
CFLAGS  +=      
#指定库
LDFLAGS  := -lm -lfreetype      

export  CFLAGS  LDFLAGS     

TOPDIR  :=  $(shell  pwd)
 

obj-y  +=  show_font/
obj-y  +=  test_freetype/


all  :   
	make  -C  ./  -f  $(TOPDIR)/Makefile.mk
#	$(CC)  $(LDFLAGS)  -o  $(TARGET)  built-in.o 


clean: 
	rm  -f  $(shell  find  -name  "*.o") 
	rm  -f  $(subst /,.out,$(obj-y)) 

distclean: 
	rm  -f  $(shell  find  -name  "*.o") 
	rm  -f  $(shell  find  -name  "*.d") 
	rm  -f  $(subst /,.out,$(obj-y))


 

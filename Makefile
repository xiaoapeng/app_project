CROSS_COMPILE  = arm-linux-
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

CFLAGS  :=  -Wall  -O0  -g    
CFLAGS 	+= 
#指定库
LDFLAGS  := -lm -lfreetype      

export  CFLAGS  LDFLAGS     

TOPDIR  :=  $(shell  pwd)
export  TOPDIR 


obj-y += show_file/
all  : 
	sudo chmod a+wr ./* -R
	make  -C  ./  -f  $(TOPDIR)/Makefile.mk 


clean: 
	rm  -f  $(shell  find  -name  "*.o") 
	rm  -f  $(subst /,.out,$(obj-y)) 

distclean: 
	rm  -f  $(shell  find  -name  "*.o") 
	rm  -f  $(shell  find  -name  "*.d") 
	rm  -f  $(subst /,.out,$(obj-y))

install:
	cp *.out /home/plz/nfs/fs_linux_3.4.2/drv/
 

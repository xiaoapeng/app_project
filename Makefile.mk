PHONY  :=  __build 
__build: 
obj-y  := 
subdir-y  := 
include  Makefile 

#假设 obj-y = a/ b/ c/ d.o e.o f.o
# 把文件夹（a/ b/ c/）提取为 （a b c）
__subdir-y  :=  $(patsubst  %/,%,$(filter  %/,  $(obj-y))) 
subdir-y  +=  $(__subdir-y) 

# 全部变为 a/built-in.o b/built-in.o c/built-in.o
subdir_objs  :=  $(foreach  f,$(subdir-y),$(f)/built-in.o) 
   

PHONY  +=  $(subdir-y) 

subdir-out := $(foreach  f,$(subdir-y),$(f).out)
   
#默认执行这条规则
__build  :  $(subdir-y)  $(subdir-out) 

   
#在子目录下执行make
$(subdir-y):
	make  -C  $@  -f  $(TOPDIR)/Makefile.build 

#使用子目录下的built-in.o 生成子目录下的.out文件
$(subdir-out) :  $(subdir_objs) 
	$(CC)  $(LDFLAGS)  -o  $@  $(subst .out,/built-in.o,$@) 

.PHONY  :  $(PHONY) 

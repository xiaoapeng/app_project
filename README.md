## 一次可编译多个程序-自用
模仿内核Makefile，支持在文件夹下Makefile中添加 obj-y += *.o
在顶层目录Makefile中添加 obj-y += */   则会编译该子目录成 .out执行文件

#最终编译生成名为pid的可执行文件
#author：景玉军
pid:pid.o
	gcc -o pid pid.o
pid.o:pid.c
	gcc -c pid.c
clean:
	rm pid.o

#清理编译过程中产生的.o文件

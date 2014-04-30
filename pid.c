#include <sys/types.h>     //pid_t数据类型的定义
#include "stdio.h"         //标准输入输出
#include <dirent.h>
#include <unistd.h>
#include "string.h"
#include "stdlib.h"
#include <ctype.h>

#define READ_BUF_SIZE    50  //读缓存大小
// pid 的获得函数（由于进程都在目录/proc下，有相对应的文件）
pid_t * find_pid_by_name( char* pidName)
{
    DIR *dir;
    struct dirent *next;
    pid_t* pidList=NULL;
    int i=0;
    dir = opendir("/proc");
    while ((next = readdir(dir)) != NULL) {
        FILE *status;
        char filename[READ_BUF_SIZE];
        char buffer[READ_BUF_SIZE];
        char name[READ_BUF_SIZE];
        /* Must skip ".." since that is outside /proc */
        if (strcmp(next->d_name, "..") == 0)
            continue;

        /* If it isn't a number, we don't want it */
        if (!isdigit(*next->d_name))
            continue;

        sprintf(filename, "/proc/%s/status", next->d_name);
        if (! (status = fopen(filename, "r")) ) {
            continue;
        }
        if (fgets(buffer, READ_BUF_SIZE-1, status) == NULL) {
            fclose(status);
            continue;
        }
      fclose(status);

        /* Buffer should contain a string like "Name:   binary_name" */
        sscanf(buffer, "%*s %s", name);

        if (strcmp(name, pidName) == 0) {
            pidList=realloc( pidList, sizeof(pid_t) * (i+2));
            pidList[i++]=strtol(next->d_name, NULL, 0);
        }
    }
    return pidList;
}

//主函数调用，函数的调用形式为：比如： ./pid firefox  即可输出相对应进程的id号
int main(int argc,char *argv[])
{
	pid_t * pidList;
	if(argc==2)
		pidList = find_pid_by_name(argv[1]);
	else
		printf("输入错误！");
	printf("the program's pid is:%d\n",*pidList);
	return 0;
}

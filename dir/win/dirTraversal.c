#include "dirTraversal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef WIN32 // for linux
#include <sys/dir.h>
#include <sys/stat.h>
#else // for windows 
#include <io.h>
#endif //WIN32



#ifndef WIN32 // for linux
inline int isDir(const char* path)
{
         struct stat st;
         lstat(path, &st);
         return S_ISDIR(st.st_mode);
}

int dirTraversal(const char *path, int recursive)
{
         DIR *pdir;
         struct dirent *pdirent;
         char temp[256];
         pdir = opendir(path);
         if(pdir)
         {
                 while((pdirent = readdir(pdir)) != 0)
                 {
                           //跳过"."和".."
                           if(strcmp(pdirent->d_name, ".") == 0
                                     || strcmp(pdirent->d_name, "..") == 0)
                                   continue;
                           sprintf(temp, "%s/%s", path, pdirent->d_name);
                           printf("%s\n", temp);
                           //当temp为目录并且recursive为1的时候递归处理子目录
                           if(isDir(temp) && recursive)
                           {
                                   dirTraversal(temp, recursive);
                           }
                 }
          }
         else
         {
                  printf("opendir error:%s\n", path);
          }
          closedir(pdir);
}
//interface
int fileList(const char *path, int recursive)
{
          int len;
          char temp[256];
          len = strlen(path);
          strcpy(temp, path);
          if(temp[len - 1] == '/') temp[len -1] = '\0';
 
          if(isDir(temp))
          {
                   //处理目录
                   dirTraversal(temp, recursive);
          }
         else   //输出文件
         {
                  printf("%s\n", path);
         }
         return 1;
}


#else //for windows
/**
 * dir traversal , for windows , references: 
 * http://wenku.baidu.com/view/9a321f23ccbff121dd368399.html
 * 
 * SIGH: unsigned
 *
 */
int dirTraversal(const char *path, int recursive)
{
	long handle;//用于查找的句柄
	struct _finddata_t fileinfo;//文件信息的结构体
	handle=_findfirst(path，&fileinfo);         //第一次查找
	if(-1==handle)return -1;
	printf("%s\n"，fileinfo.name);//打印出找到的文件的文件名
	while(!_findnext(handle，&fileinfo))//循环查找其他符合的文件，知道找不到其他的为止
	{
		printf("%s\n"，fileinfo.name);
	}
	_findclose(handle);//关闭句柄
	return 1;
}

#endif //end of linux/windows

int main(int argc, char** argv)
{
	const char *dir = "test/";
	fileList(dir, 1);
	return 0;
}

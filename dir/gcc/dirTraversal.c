#include "dirTraversal.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/dir.h>
#include <sys/stat.h>
#include <string.h>

int readData(const char *filename)
{
	FILE *fp = fopen(filename,"r");
	char ch;
	if(fp == NULL) return 0;
	printf("%s:\n",filename);
	while((ch = fgetc(fp)) != EOF)
	{
		putchar(ch);
	}
	printf("\n==============================================================\n");
	fclose(fp);
	return 1;
}


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
                           //printf("%s\n", temp);
                           readData(temp);
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
        return 1;
}

int fileList(const char *path, int recursive)
{
          int len;
          char temp[256];
          //去掉末尾的'/'
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


int main(int argc, char** argv)
{
	const char *dir = "test/";
         fileList(dir, 1);
         return 0;
}

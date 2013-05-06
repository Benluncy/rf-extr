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

//
int doTraversal(const char *path, int recursive,file_callback xCallback)
{
	DIR *pdir;
	struct dirent *pdirent;
	char tmp[1024];
	pdir = opendir(path);
	if(pdir)
	{
		while((pdirent = readdir(pdir)) != 0)
		{
			//ignore "." && ".."
			if(!strcmp(pdirent->d_name, ".")|| 
				!strcmp(pdirent->d_name, "..")) continue;
			sprintf(tmp, "%s/%s", path, pdirent->d_name);
			xCallback(tmp,isDir(tmp));
			//if is Dir and recursive is true , into recursive
			if(isDir(tmp) && recursive)
			{
				doTraversal(tmp, recursive,xCallback);
			}
		}
	}else
	{
		fprintf(stderr,"opendir error:%s\n", path);
	}
	closedir(pdir);
	return 1;
}

//interface
int dirTraversal(const char *path, int recursive,file_callback xCallback)
{
	int len;
	char tmp[256];
	len = strlen(path);
	strcpy(tmp, path);
	if(tmp[len - 1] == '/') tmp[len -1] = '\0';

	if(isDir(tmp))
	{
		doTraversal(tmp, recursive,xCallback);
	}
	else
	{
		//printf("%s\n", path);
		xCallback(path,isDir(path));
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
int tab = 1;
int dirTraversal(const char *path, int recursive,file_callback xCallback)
{
	int len = strlen(path)+3;
	long handle;//用于查找的句柄
	char mypath[len+3];
	char tmppath[len];
	char nxtpath[len+1024];
	char sp = 0;
	int i;
	struct _finddata_t fileinfo;//文件信息的结构体
	sprintf(mypath,"%s",path);
	switch(mypath[len-1])
	{
		case '\\':
			sp = '\\';
			len -= 1;
			mypath[len-1] = '\0';
			break;
		case '/':
			len -= 1;
			mypath[len-1] = '\0';
		case '.':
			sp = '/';
			break;
		default :
			for(i=0;i<len;i++)
			{
				if(mypath[i]=='\\'||mypath[i]=='/')
				{
					sp = mypath[i];
					break;
				}
			}
	}
	sprintf(tmppath,"%s",mypath);
	printf("path:%s - %s \n",tmppath, mypath);
	sprintf(mypath,"%s%c%s",mypath,sp,"*");
	printf("%s\n",mypath);
	system("pause");
	handle=_findfirst(mypath,&fileinfo);//第一次查找
	if(-1==handle) return -1;
	do
	{
		for(i=0;i<tab;i++) printf("    ");
		printf("+-");
		printf("%s\n",fileinfo.name);
		if(((fileinfo.attrib & _A_SUBDIR)!=0) && recursive && fileinfo.name[0]!='.')
		{
			printf("0:%c1:%c2:%c\n",fileinfo.name[0],fileinfo.name[1],fileinfo.name[2]);
			if(getchar() == 'q') return 1;
			tab++;
			printf("-->\n");
			sprintf(nxtpath,"%s%c%s",tmppath,sp,fileinfo.name);
			printf("[%s]\n",nxtpath);
			dirTraversal(nxtpath,1);
			printf("<--\n");
			tab--;
		}
		
	}while(!_findnext(handle,&fileinfo));//循环查找其他符合的文件,知道找不到其他的为止
	_findclose(handle);//关闭句柄
	return 1;
}

#endif //end of linux/windows

/**
int main(int argc, char** argv)
{
//	const char *dir = "E:\\tmp\\dir\\win\\";
	const char *dir = ".";
	dirTraversal(dir, 1);
	return 0;
}
*/

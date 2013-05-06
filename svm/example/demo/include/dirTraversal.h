#ifndef __DIR_TRAVERSAL_H__
#define __DIR_TRAVERSAL_H__
/*
struct _fileList
{
	struct _fileList *
};
typedef _fileList * fileList;
*/

typedef int (*file_callback)(const char* fileName,int isDir);

//int dirTraversal(const char *path, int recursive,int file_callback);
int dirTraversal(const char *path, int recursive,int (*callback)(const char* fileName,int isDir));


#endif //__DIR_TRAVERSAL_H__

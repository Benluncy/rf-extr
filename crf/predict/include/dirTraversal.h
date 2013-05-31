#ifndef __DIR_TRAVERSAL_H__
#define __DIR_TRAVERSAL_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#ifndef WIN32 // for linux
int isDir(const char* path);
#else // for windows

#endif // enf for Linux or Windows

typedef int (*file_callback)(const char* fileName,int isDir);

//int dirTraversal(const char *path, int recursive,int file_callback);
int dirTraversal(const char *path, int recursive,int (*callback)(const char* fileName,int isDir));

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //__DIR_TRAVERSAL_H__

#include <stdio.h>
#include "dao.h"

#define RES_LEN 3000

int main()
{
	// DB test
	int a[RES_LEN];
	int alen;
	
	// 一次查询
	dbconnect(); // 连接数据库
	dblock(); // 事务锁 -- 独占数据库，在内存中处理
	alen = getcoau(1423262,a,RES_LEN); // 查询: author id 为: 1423262
	printf("len is:%d\nvalues are:\n",alen); // alen 是查询得到的长度
	for(int i=0;i<alen;i++) // 打印
		printf("%d ",a[i]);
	fflush(NULL); //
	dbunlock(); // 解事务锁
	dbfree(); // 断开数据库
	
	
	//1000000次查询
	dbconnect();
	dblock();
	for(int i=0;i<1000000;i++) // 100W次查询
	{
		getcoau(i,a,300);
	}
	dbunlock();
	dbfree();
	//[yu@argcandargv-com php]$ time ./dbquery 

	//real	0m33.482s
	//user	0m33.368s
	//sys	0m0.053s

	return 0;

}



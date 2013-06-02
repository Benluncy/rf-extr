/**
 * The MIT License (MIT)
 *
 *
 * Copyright (C) 2013 Yu Jing (yujing5b5d@gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute,sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED,INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */
/**
 *  Minumum edit distance
 *  
 *  Author : 
 *	Yu Jing <yujing5b5d@gmail.com>
 *
 *  Modified :
 *	
 *
 *  Last Updated :
 *	Mar 19 , 2013	: create 
 *	
 *  Reference :
 *	http://ccl.pku.edu.cn/doubtfire/Course/Computational%20Linguistics/contents/Minimum%20Edit%20Distance.pdf
 *
 */
#include "minEditDistance.h"
#include <stdio.h>
#include <string.h>

#ifndef INSERT_COST
#define INSERT_COST 1
#endif // INSERT_COST

#ifndef DELETE_COST
#define DELETE_COST 1
#endif // DELETE_COST

#ifndef SUBSTITUTE_COST 
#define SUBSTITUTE_COST 1
#endif // SUBSTITUTE_COST

#ifndef _MIN
#define _MIN(x,y,z) ((x<y)?((x<z)?x:z):((y<z)?y:z))
#endif // _MIN(x,y,z)

inline int isSame(char a,char b) 
{
	return ((a <= 'Z' && a >= 'A') ? (a +32) : (a))	
		== ((b <= 'Z' && b >= 'A') ? (b + 32) : (b)) ; 
}

//minimum_edit_distance
inline int editDistance(const char *t,const char * s) //compare target && src
{
	int m = strlen(s)+1; // length of source + 1 for j in 0,1,2...m-1
	int n = strlen(t)+1; // length of dest + 1 for i in 0,1,2...n-1
	int matrix[n][m];// distance matrix
	int i;
	int j;
	for(i = 0 ; i < n ; i ++)
	{
		memset(matrix[i],0,m*sizeof(int));
		matrix[i][0] = i;
	}
	for(i = 0 ; i < m;i++) matrix[0][i] = i;
	for(i = 1 ; i < n ; i ++ )
	{
		for(j = 1 ; j < m ; j ++)
		{
			if(t[i-1] == s[j-1])
			{
				matrix[i][j] = matrix[i-1][j-1];
			}else
			{
				matrix[i][j] = _MIN(matrix[i][j-1] + INSERT_COST, // insert cost
						matrix[i-1][j] + DELETE_COST, // delete cost 
						matrix[i-1][j-1] + SUBSTITUTE_COST); // substitute cost
			}
		}
	}			
	return matrix[n-1][m-1];
}



inline int editDistanceS(const char *t,const char * s) //compare target && src
{
	int m = strlen(s)+1; // length of source + 1 for j in 0,1,2...m-1
	int n = strlen(t)+1; // length of dest + 1 for i in 0,1,2...n-1
	int matrix[n][m];// distance matrix
	int i;
	int j;
	for(i = 0 ; i < n ; i ++)
	{
		memset(matrix[i],0,m*sizeof(int));
		matrix[i][0] = i;
	}
	for(i = 0 ; i < m;i++) matrix[0][i] = i;
	for(i = 1 ; i < n ; i ++ )
	{
		for(j = 1 ; j < m ; j ++)
		{
			if(isSame(t[i-1],s[j-1]))
			{
				matrix[i][j] = matrix[i-1][j-1];
			}else
			{
				matrix[i][j] = _MIN(matrix[i][j-1] + INSERT_COST, // insert cost
						matrix[i-1][j] + DELETE_COST, // delete cost 
						matrix[i-1][j-1] + SUBSTITUTE_COST); // substitute cost
			}
		}
	}			
	return matrix[n-1][m-1];
}


/*
int main(int argc ,char *argv[])
{
	if(argc != 3)
	{
		printf("sot:stop is : %d\n",editDistance("sot","stop"));
		printf("ykyyyab<==>abxxxxx is : %d\n",editDistance("ykyyyab","abxxxxx"));
		printf("rr is : %d\n",editDistanceS("References","REFERENCES"));
	}else
	{
		printf("%s<=>%s ? %d(%d) \n",argv[1],argv[2],
			editDistance(argv[1],argv[2]),
			editDistanceS(argv[1],argv[2]));
	}
	return 0;
}
*/



























































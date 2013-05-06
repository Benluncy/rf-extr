Minimum edit distance 最小编辑距离
====================================================
calcuate the minumum edit distance

with log http://argcandargv.com/archives/1619


References:
====================================================
<ul>
<li>[1] 如何计算最小编辑距离 http://ccl.pku.edu.cn/doubtfire/Course/Computational%20Linguistics/contents/Minimum%20Edit%20Distance.pdf</li>
<li>[2] Minimum Edit Distance http://www.stanford.edu/class/cs124/lec/med.pdf</li>
</ul>

BLOG
====================================================
Permalink : http://argcandargv.com/archives/1619

<h2 id="h20">需求</h2>
现在有两个字符串，我们怎样衡量它们之间的"相似度"呢？比如naxt和exnt哪个和next更加相似呢？这就需要一个统一的衡量准则了。

为了做一个统一的衡量准则，有人提出了<a href="http://zh.wikipedia.org/wiki/%E7%B7%A8%E8%BC%AF%E8%B7%9D%E9%9B%A2" title="编辑距离">编辑距离</a>(<a href="http://en.wikipedia.org/wiki/Edit_distance" title="Edit distance">Edit distance</a> or <a href="http://en.wikipedia.org/wiki/Levenshtein_distance" titie="Levenshtein distance">Levenshtein distance</a>)这个概念。

什么叫编辑距离？如字面意思，就是从字符串a(source)到字符串b(target)需要经过多少次编辑。每次编辑只能增加/删除/替换1个字符。计算出从a到b最小的编辑距离(Minimum edit distance)，用此可作为两个字符串之间的相似度的衡量。

当字符串a和字符串b相同的时候，编辑距离为0；
两者差异越大，编辑距离越大。

<h2 id="h21">原理说明</h2>
一般情况下，求编辑距离是使用一个a长度*b长度的二位数组作为计算区域，用<a href="http://zh.wikipedia.org/wiki/%E5%8A%A8%E6%80%81%E8%A7%84%E5%88%92" title="动态规划">动态规划</a>(<a href="http://en.wikipedia.org/wiki/Dynamic_programming">Dynamic Programming</a> 简称DP)的方法求取的。

具体步骤是:
1. 建立个m*n的矩阵matrix，其中m=len(a)+1,n=len(b)+1。这样，二维数组的范围就是 matrix[0...n-1][0...m-1]了。
2. 给matrix[0][0...m-1]赋值为0...m-1,matrix[0...n-1][0]赋值为0...n-1
3. 遍历matrix[1...n-1][1...m-1]
matrix每个元素的值都为:<a id="getmin"/>
[latex]min\left\{\begin{array}{lll}matrix[i-1][j]+INSERT\_COST \\matrix[i][j-1]+DELETE\_COST\\matrix[i-1][j-1]+SUBSTITUTE\_COST(source_i, target_j)\end{array} \right.[latex]

其中，INSERT_COST，DELETE_COST 都是固定值(可以自己设定1,2什么的)。
而SUBSTITUTE_COST是需要判断的。
当[latex]source_i[/latex]和[latex]target_i[/latex]的值是等价的时候，SUBSTITUTE_COST就是0,否则SUBSTITUTE_COST就是预定义的一个权值。

最后matrix[n-1][m-1]的值则为两个字符串的最小编辑距离。

<strong>改进</strong>
当[latex]source_i[/latex]和[latex]target_i[/latex]的值相同时，SUBSTITUTE_COST的值就是0，必然是最小值。所以首先判断两字符是否相等，若相等则直接判定matrix[i][j]=matrix[i-1][j-1]，判断下个。这样可以省很多计算。


<h2 id="h22">实现</h2>
<h3 id="h30">预定义</h3>
插入操作的权值(INSERT_COST),删除操作的权值(DELETE_COST)和替换操作的权值(SUBSTITUTE_COST)都是可以自己定义的。这儿我把它们用预处理分别定义为1,1和2。但其实还可以有其他方法，比如用变量定义什么的。
[code lang="c"]
#ifndef INSERT_COST
#define INSERT_COST 1
#endif // INSERT_COST

#ifndef DELETE_COST
#define DELETE_COST 1
#endif // DELETE_COST

#ifndef SUBSTITUTE_COST 
#define SUBSTITUTE_COST 2
#endif // SUBSTITUTE_COST
[/code]

<h3 id="h31">工具宏</h3>
<a href="#getmin">原理</a>已经说明过了，需要求三者之最小值，所以直接用一个宏来完成“求取三者最小值”的功能。不同于递归，宏定义消耗很小，完全可以放心使用。

[code]
#ifndef _MIN(x,y,z)
#define _MIN(x,y,z) ((x<y)?((x<z)?x:z):((y<z)?y:z))
#endif // _MIN(x,y,z)
[/code]

<h3 id="h32">主逻辑</h3>
[code lang="c" highlight="18"]
int editDistance(const char *t,const char * s) //compare target && src
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
[/code]

<h3 id="h33">扩展</h3>

L18高亮部分就是判断字符t[i-1]和s[j-1]是否相等的。
如果我们有什么特别的要求的话，可以在这儿做点手脚。
比如，我们希望让'a'和'b'被看成是同一个东西。
那么，我们做个判断函数
[code lang="c"]
inline int isSame(char a, char b)
{
	return (a<b?(a=='a'&&b=='b'):(a=='b'&&b=='a'))|| a==b;
}
[/code]

再把L18修改为
[code lang="c"]
if(isSame(t[i-1],s[j-1]))
[/code]
当两个字符分别为'a'和'b'的时候，也被判断为TRUE,
大小写不敏感之类其他的判断也大抵如此。



github中有完整代码，地址:<a href="https://github.com/yujing5b5d/minEditDistance/" title="minEditDistance">https://github.com/yujing5b5d/minEditDistance/</a>


<h2>References</h2>
[1] 如何计算最小编辑距离 http://ccl.pku.edu.cn/doubtfire/Course/Computational%20Linguistics/contents/Minimum%20Edit%20Distance.pdf
[2] Minimum Edit Distance http://www.stanford.edu/class/cs124/lec/med.pdf

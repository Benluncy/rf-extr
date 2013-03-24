/**
 * The MIT License (MIT)
 *
 *
 * Copyright (C) 2013 Yu Jing <yujing5b5d@gmail.com>
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

#include "demo.h"
#include "sqlite3.h" //#include "sqlite3ext.h"  //only need one of the head , base on need

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <stdarg.h>

/**
   typedef struct sqlite3 sqlite3;
   int sqlite3_open(const char*, sqlite3**);
   int sqlite3_open16(const void*, sqlite3**);
   int sqlite3_close(sqlite3*);
   const char *sqlite3_errmsg(sqlite3*);
   const void *sqlite3_errmsg16(sqlite3*);
   int sqlite3_errcode(sqlite3*);
 */

/**
//sqlite 3 , errorcode :
#define SQLITE_OK           0   // Successful result
#define SQLITE_ERROR        1   // SQL error or missing database
#define SQLITE_INTERNAL     2   // An internal logic error in SQLite
#define SQLITE_PERM         3   // Access permission denied
#define SQLITE_ABORT        4   // Callback routine requested an abort
#define SQLITE_BUSY         5   // The database file is locked
#define SQLITE_LOCKED       6   // A table in the database is locked
#define SQLITE_NOMEM        7   // A malloc() failed
#define SQLITE_READONLY     8   // Attempt to write a readonly database
#define SQLITE_INTERRUPT    9   // Operation terminated by sqlite_interrupt()
#define SQLITE_IOERR       10   // Some kind of disk I/O error occurred
#define SQLITE_CORRUPT     11   // The database disk image is malformed
#define SQLITE_NOTFOUND    12   // (Internal Only) Table or record not found
#define SQLITE_FULL        13   // Insertion failed because database is full
#define SQLITE_CANTOPEN    14   // Unable to open the database file
#define SQLITE_PROTOCOL    15   // Database lock protocol error
#define SQLITE_EMPTY       16   // (Internal Only) Database table is empty
#define SQLITE_SCHEMA      17   // The database schema changed
#define SQLITE_TOOBIG      18   // Too much data for one row of a table
#define SQLITE_CONSTRAINT  19   // Abort due to contraint violation
#define SQLITE_MISMATCH    20   // Data type mismatch
#define SQLITE_MISUSE      21   // Library used incorrectly
#define SQLITE_NOLFS       22   // Uses OS features not supported on host
#define SQLITE_AUTH        23   // Authorization denied
#define SQLITE_ROW         100  // sqlite_step() has another row ready
#define SQLITE_DONE        101  // sqlite_step() has finished executing
*/

typedef sqlite3 * _hdb;
char _m_query[1024];
static _hdb db;
static FILE * db_report;
int connect_db(const char *file_name);
int create_table(const char *table_name,int num,char *column_name[100]);

#define PREPARE(x) sprintf(_m_query,"%s",x);
#define QUERY _m_query

void one_row(int n_values, char** values)
{
    int i;
    for (i = 0; i < n_values; ++i)
    {
        if (i > 0)
        {
            printf("\t");
        }
        printf("%20s", values[i]);
    }
    printf("\n");
}

int result_handle(void* data,int n_columns,char** column_values,char** column_names)
{
    static int column_names_printed = 0;
    //int i;
    if (!column_names_printed)
    {
        one_row(n_columns, column_names);
        column_names_printed = 1;
    }

    one_row(n_columns, column_values);
    return 0;
}



int main(int argc , char * argv[])
{
    int status;
    char * str_err_msg= NULL;
    db_report = stdout;
    printf("sqlite version : %s\n%s\n",sqlite3_libversion(),sqlite3_sourceid());
    //_hdb db;
	connect_db("demo.db");
//	/**
    PREPARE("create table usrpwd(usr QString, pwd QString)");
    status = sqlite3_exec(db,
                          QUERY,
                          0,
                          0,
                          &str_err_msg);
    printf("c->status: %d ~ %s \n",status,str_err_msg);
    /* */
   // /**
    	PREPARE("insert into usrpwd values('hkkk','khhh')");
    	status = sqlite3_exec(db,
    						QUERY,
    						0,
    						0,
    						&str_err_msg);
    	printf("c->status: %d ~ %s \n",status,str_err_msg);
    //**/

    PREPARE("select * from usrpwd ");

    status = sqlite3_exec(db,
                          QUERY,
                          result_handle, // int (*callback)(void*,int,char**,char**),
                          0,
                          &str_err_msg);
    printf("c->status: %d ~ %s \n",status,str_err_msg);



    status = sqlite3_close(db);
    if(status != SQLITE_OK )
        printf("error:%d\n",sqlite3_errcode(db));
    return 0;
}

//! _hdb db;
int connect_db(const char *file_name)
{
	int status = sqlite3_open(file_name,&db);
    fprintf(db_report,"error:%d\n",sqlite3_errcode(db));
    return status ? 0 : 1;
}

//!int create_table(const char *table_name,int num,int type[],char *column_name[100])
int create_table(const char *table_name,int num,char *column_name[100]) // default : QString
{
	char query[1024];
	int i;
	sprintf(query,"create table %s{",table_name);
	for(i = 0 ; i < num; i++)
	{
		sprintf(query,"QString %s,",column_name[i]);
	}
	sprintf(query,"};");
	return 1;
}




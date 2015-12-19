/*
 * sqlite_db.cpp
 *
 *  Created on: Jul 14, 2011
 *      Author: Administrator
 */
#include "sqliteDB.hpp"
#include <string.h>

SqliteDB ::SqliteDB()
{
	db=NULL;
	q=NULL;

}
SqliteDB ::~SqliteDB()
{
	if(q)
		delete q;
	if(db)
		delete db;

}

unsigned char SqliteDB::Init(const char *dbfname)
{
	db = new Database(dbfname,&log);
	if(!db)
		return 1;
	q  = new Query(*db);
	if(!q)
		return 1;
	return 0;
}

bool SqliteDB::execute(const char *sql)
{
	return q->execute(sql);
}

short SqliteDB::get_result(const char *sql)
{
	q->get_result(sql);
	return q->num_rows();
}

bool SqliteDB::fetch_row()
{
	return q->fetch_row();
}
//int
long SqliteDB::getval()
{
	return q->getval();
}
long SqliteDB::getval(const char *col)
{
	return q->getval(col);
}

//real
double SqliteDB::getnum()
{
	return q->getnum();
}
double SqliteDB::getnum(const char *col)
{
	return q->getnum(col);
}

//string

unsigned char SqliteDB::getstr(char *result)
{
	strcpy(result,q->getstr());
	return strlen(result);
}
unsigned char SqliteDB::getstr(char *result,const char *col)
{
	strcpy(result,q->getstr(col));
	return strlen(result);
}

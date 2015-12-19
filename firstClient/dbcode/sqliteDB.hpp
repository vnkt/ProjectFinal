/*
 * sqlite_db.hpp
 *
 *  Created on: Jul 14, 2011
 *      Author: Administrator
 */

#ifndef SQLITE_DB_HPP_
#define SQLITE_DB_HPP_
#include "sqlitewrapped.h"

class SqliteDB
{
private:
	Database *db;
	StderrLog log;
	Query *q;
public:
	SqliteDB();
	~SqliteDB();

	unsigned char Init(const char *dbfname);
	bool execute(const char *sql);
	short get_result(const char *sql);
	void free_result(){q->free_result();};

	bool fetch_row();

	//retrieve integer
	long getval();                 //return next col
	long getval(const char *col);  //specify col name

	//retrieve float
	double getnum();
	double getnum(const char *col);

	//string
	unsigned char getstr(char *result);
	unsigned char getstr(char *result,const char *col);



};

#endif /* SQLITE_DB_HPP_ */

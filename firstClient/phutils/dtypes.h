/*
 * dtypes.h
 *
 *  Created on: Jan 3, 2011
 *      Author: Administrator
 */

#ifndef DTYPES_H_
#define DTYPES_H_

typedef unsigned short usint;
typedef short           sint;
typedef unsigned char flag_t;
typedef unsigned char            byte;
typedef unsigned int    uint;
typedef unsigned short int word;
typedef unsigned int dword;
typedef unsigned int lword;

typedef short int			index_t;
typedef long unsigned int	addr_t;
typedef unsigned short int  err_t;
typedef unsigned short int  sword;

typedef unsigned int status_t;


#define OK 0
#define NOT_OK 1
#define TRUE 1
#define FALSE 0
#define NO_ERR 0
#define ERROR 1
#define INVALID -1
#define ENABLE 1
#define DISABLE 0

#define MAX_LINE 50
#define MAX_TAG_LEN 10;


#define _MAX_FNAME 75
/*

typedef union {
				unsigned long int laddr;
				byte     addr[4];
				byte     saddr[2];
			  } obc_addr_t;

typedef union
	{
	usint data_wd;
	struct
		{
		 char	by0;
		 char 	by1;
		} data_by;
	} mdata_t;
	*/

#define CASE(k) ( (k)[0]<<8 | (k)[1])


#endif /* DTYPES_H_ */

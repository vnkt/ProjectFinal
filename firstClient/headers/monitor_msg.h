/*
 * monitor_msg.h
 *
 *  Created on: Dec 28, 2011
 *      Author: user
 */

#ifndef MONITOR_MSG_H_
#define MONITOR_MSG_H_
#include <sys/neutrino.h>

#define MSG_MAX 3
#define MAX_PKG 3

typedef int STATUS;
typedef int MSGTYPE;

typedef struct
{
	MSGTYPE msgType;
}start_msg;

typedef struct
{
	MSGTYPE msgType;
	STATUS status;
}start_reply;

typedef struct
{
	MSGTYPE msgType;
	unsigned short RT_No;
	unsigned short store_data;
	char filename[30];
	unsigned short procId;
	//minor data details
	unsigned short frame_enclose;
	unsigned char  frame_start_char,frame_end_char;
	unsigned char  insert_dummy_chars;
	unsigned char  num_dummy_chars;   //dummy chars to be inserted at the beginning
	unsigned short msgsts_word_pos;   //within dummy char
	unsigned short total_msg;         //
	unsigned short word_count[MSG_MAX];
	unsigned short Subaddress[MSG_MAX];
	unsigned short minid_pos;    //position of minor id in tele-metry
	//major data details
	unsigned short total_minor_cycles;
	 //minor cycles per major
} monitor_package_details_t;


typedef struct
{
	MSGTYPE msgType;
	unsigned char store_full_bus_data;
	char bus_data_filename[30];
	unsigned short num_pkg;     //total number of packages
	monitor_package_details_t pkg_details[MAX_PKG];
}config_msg_t;


typedef struct
{
	MSGTYPE msgType;
	STATUS status;
	int objectNumber;
}config_reply;

typedef struct
{
	MSGTYPE msgType;
}stop_msg;

typedef struct
{
	MSGTYPE msgType;
	STATUS status;
}stop_reply;

typedef struct
{
	MSGTYPE msgType;
	int objNumber;
}real_msg;

typedef struct
{
	MSGTYPE msgType;
	STATUS status;
	unsigned char reply[5000];
	char error[50];
	int count;
	int fileSize;
}real_reply;

typedef struct
{
	MSGTYPE msgType;
}current_status_req;

typedef struct
{
	MSGTYPE msgType;
	STATUS status;
	int noRTAlive;
	int noOfMessages;
	int noOfValidMessages;
	int noOfInvalidMessages;
	int rtSubAddrMessageCount[32][32];
	int rtInvalidSubAddrMessageCount[32][32];
	int fileSize;
}current_status_reply;

typedef struct
{
	MSGTYPE msgType;
	char filename[50];
	int writeNeeded;
}start_wo_msg;

typedef struct
{
	MSGTYPE msgType;
}start_wo_reply;

typedef union
{
	MSGTYPE msgType;
	start_msg startMsg;
	start_reply startReply;
	start_wo_msg startWoMsg;
	start_wo_reply startWoReply;
	config_msg_t configMsg;
	config_reply configReply;
	stop_msg stopMsg;
	stop_reply stopReply;
	real_msg realReq;
	current_status_req currentReq;
}monitor_msg;

typedef struct
{
	_pulse pulse;
	monitor_msg message;
}monitor_buff;

enum{START_MSG,START_REPLY,CONFIG_MSG,CONFIG_REPLY,STOP_MSG,STOP_REPLY,REAL_REQ,REAL_REPLY,CURR_REQ,CURR_REPLY,START_WO_REQ};

#define SERVER "SERVER"
#define TIMER "TIMER_SERVER"
#endif /* MONITOR_MSG_H_ */

/* 	sercli.cpp
	Functions to interact with serial servers..
*/
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <process.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <sys/sched.h>
#include "sercli.hpp"
#include "monitor_msg.h"

int MonClient::ToSvr()
{
	int i;
	if(msg.message.msgType == REAL_REQ)
		i = MsgSend(coid,&msg,sizeof(msg),&realtimeData,sizeof(realtimeData));

	if(msg.message.msgType == CURR_REQ)
		i = MsgSend(coid,&msg,sizeof(msg),&currentData,sizeof(currentData));
	else
		i = MsgSend(coid,&msg,sizeof(msg),&reply,sizeof(reply));

	if (i != 0)
	{
		coid = connect();
		if(msg.message.msgType == REAL_REQ)
			i = MsgSend(coid,&msg,sizeof(msg),&realtimeData,sizeof(realtimeData));
		if(msg.message.msgType == CURR_REQ)
			i = MsgSend(coid,&msg,sizeof(msg),&currentData,sizeof(currentData));
		else
			i = MsgSend(coid,&msg,sizeof(msg),&reply,sizeof(reply));
	}

 return i;
}

int MonClient::addconfig(monitor_package_details_t config)
{
	msg.message.msgType = CONFIG_MSG;
	msg.message.configMsg.pkg_details[msg.message.configMsg.num_pkg] = config;
	msg.message.configMsg.num_pkg ++;
	return 0;
}

int MonClient::setupconfig()
{
	msg.message.msgType = CONFIG_MSG;
	ToSvr();
	return 0;
}

int MonClient::setupstart()
{
	msg.message.msgType =  START_MSG;
	printf("\n Start ");
	fflush(stdout);
	ToSvr();
	if(reply.message.msgType != START_REPLY || reply.message.startReply.status != 1)
	{
		printf("\nStart failed or already running");
		fflush(stdout);
	}
	return 0;
}

int MonClient::setupstop()
{
	msg.message.msgType =  STOP_MSG;
	ToSvr();
	if(reply.message.msgType != STOP_REPLY || reply.message.startReply.status == 0)
	{
		printf("\n Unable to stop");
		fflush(stdout);
	}
	return 0;
}

int MonClient::setupreal(int objectNumber)
{
	msg.message.msgType = REAL_REQ;
	msg.message.realReq.objNumber = objectNumber;
	ToSvr();
	return 0;
}

int MonClient::setupcurrent()
{
	msg.message.msgType = CURR_REQ;
	ToSvr();
	return 0;
}

int MonClient::setupstartwo(char *file, int size, int needed)
{
	msg.message.msgType =  START_WO_REQ;
	if(needed == 1)
	{
		memcpy(msg.message.startWoMsg.filename, file, size);
		msg.message.startWoMsg.writeNeeded = 1;
	}
	else
		msg.message.startWoMsg.writeNeeded = 0;

	ToSvr();
	if(reply.message.msgType != START_REPLY || reply.message.startReply.status != 1)
	{
		printf("\nStart failed or already running");
		fflush(stdout);
	}
	return 0;
}

int  MonClient::test()
{
	if(coid == -1)
		coid = connect();
	return coid;
}

int MonClient::outputbuf(byte *input)
{
	if(realtimeData.status == 1)
	{
		memcpy(input,realtimeData.reply,realtimeData.count);
		return realtimeData.count;
	}
	else
		return 0;
}

pid_t MonClient::connect()
{
	char pname[20];
	strcpy(pname,SERVER);
	return(name_open(pname,NAME_FLAG_ATTACH_GLOBAL ));
}

int MonClient::Init()
{
	coid = connect();
	if(coid == -1)
	{
		printf("\n Error in connection ");
		return -1;
	}
	else
		return coid;
}

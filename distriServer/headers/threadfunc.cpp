/*
 * threadfunc.cpp
 *
 *  Created on: Feb 21, 2012
 *      Author: user
 */

#include "threadfunc.h"
#include "unistd.h"
#include "extern.hpp"

#include <stdlib.h>
int channelId;
int conId;

void setUpTimer()
{
	int suc;
	timer_t timerId;
	struct sigevent event;
	struct itimerspec timer;

	conId = ConnectAttach(0,0,channelId,0,0);
	if(conId == -1)
		exit(0);
	SIGEV_PULSE_INIT(&event,conId,10,_PULSE_CODE_MINAVAIL,0);
	suc = timer_create(CLOCK_REALTIME,&event,&timerId);
	if(suc == -1)
		exit(0);
	timer.it_value.tv_sec = 1;
	timer.it_value.tv_nsec = 0;
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_nsec = 1000000;
	suc = timer_settime(timerId,0,&timer,NULL);
	if(suc == -1)
		exit(0);
}

int init_monitor_memory()
{
	/* Closes the pipe for buffering */
	int pipe_create;
	pipe_create = pipe(fd);

	if(pipe_create != 0)
		exit(0);
	return 0;
}

int data_check(tagMSGRESULT &tag)
{
	int check = 0;
	tagCMDWRD cmd;

	/* type casting the command word for getting the terminal address */
	cmd =  *reinterpret_cast<tagCMDWRD*>(&tag.usCmdWrd);
	Current.noOfMessages++;
	/* 8100 is the valid block status word - if its not 8100 then fault message */
	if(tag.usBlkStsWrd != 0x8100 && tag.usBlkStsWrd != 0x8180)
	{
		Current.rtInvalidSubAddrMessageCount[cmd.RTAddress][cmd.SubAddr_ModeCode]++;
		Current.noOfInvalidMessages++;
		return -1;
	}
	/* The message is valid */
	Current.noOfValidMessages++;
	/* Checking the RT if new or old RT out of possible 32 RTs */
	for(int j =0 ; j < 32; j++)
	{
		/* Checking if the count of all sub address is -1. If its not -1 then the sub address has been already live */
		if(Current.rtSubAddrMessageCount[cmd.RTAddress][j] != -1)
		{
			check = 1;
			break;
		}
	}
	/* All sub address are -1 and hence new RT */
	if(check == 0)
		Current.noRTAlive ++;
	/* Incrementing the corresponding count of sub address pf RT */
	Current.rtSubAddrMessageCount[cmd.RTAddress][cmd.SubAddr_ModeCode]++;
	return 0;
}

/*
 * Thread functions
 *
 * Functions for data acquisition and data storage
 *	The threads call data_acquisition and data_storage respectively.
 *	The data_acquisition calls put_monitor_data function infinitely and
 *		data_storage calls write_monitor_data infinitely.
 *
 * */

void write_monitor_data()
{
	/*function for reading the tag from the pipe and writing into the disk via the class objects */
	int size_read;
    tagMSGRESULT buffer;
    tagCMDWRD cmdWord;
    unsigned short globalWriteBuffer[50];
    int wordCount;

    /* reading from the pipe */
    size_read = read( fd[0],&buffer ,sizeof(tagMSGRESULT));
	if( size_read == -1 )
		return;
	if(writeGlobal == 1)
	{
		/* global write of bus data needed */
		cmdWord = *reinterpret_cast<tagCMDWRD*>(&buffer.usCmdWrd);
		/* global bus data writing format */
		if(cmdWord.WrdCnt_ModeCode == 0)
			wordCount = 32;
		else
			wordCount = cmdWord.WrdCnt_ModeCode;
		globalWriteBuffer[0] = buffer.usBlkStsWrd;
		globalWriteBuffer[1] = buffer.usTimeTagLSW;
		globalWriteBuffer[2] = buffer.usCmdWrd;
		globalWriteBuffer[3] = wordCount + 1;
		if(cmdWord.Tx_Rx == 0)
		{
			/* Receive and data comes before status */
			for(int count = 0; count < wordCount; count ++ )
				globalWriteBuffer[4 + count] = buffer.usData[count];
			globalWriteBuffer[4 + wordCount] = buffer.usStsWrd1;
		}
		else
		{
			/* Transmit and data comes after status */
			globalWriteBuffer[4] = buffer.usStsWrd1;
			for(int count = 0; count < wordCount; count ++ )
				  globalWriteBuffer[5 + count] = buffer.usData[count];
		}
		globalWriteBuffer[5 + wordCount] = (5+wordCount) * 2;
		fwrite(globalWriteBuffer, ( 6 + wordCount) * 2, 1,globalfile);
	}
	/* sending the tag read to all the package object for storage if needed */
	for(int i = 0; i < config_count;i++)
		configuration[i].Proc_Msg(buffer);
}

int put_monitor_data(tagMSGRESULT &tag)
{
	/* function writing into pipe */
	int size_written;

	/* write the read tag into the pipe */
	size_written = write(fd[1], &tag, sizeof(tagMSGRESULT));
	if( size_written != sizeof(tagMSGRESULT))
		  printf( "\nError writing " );

	return 0;
}

void* data_acquisition(void *a)
{
	/* function called by the read thread */
	int recvId;
	FILE *fp;
	size_t answer;
	tagMSGRESULT read;
	monitor_buff message;

	channelId = ChannelCreate(0);
	setUpTimer();
	/* opening the input file for simulating of data receive from the adapter */
	fp = fopen("TEL1.DAT","r");
	while(true)
	{
		/* signal at the end of 20 milliseconds */
		recvId = MsgReceive(channelId,&message,sizeof(monitor_buff),NULL);
		if(recvId == -1)
			printf("\nReceive failed");
		else if(recvId == 0)
		{
			if(message.pulse.code == _PULSE_CODE_MINAVAIL)
			{
				/* read the data from the simulated input */
				answer = fread(&read,sizeof(tagMSGRESULT),1,fp);
				/* re - open file at end of file to infinitely simulate input */
				if(answer == 0)
					continue;
				/* check the read tag for correctness */
				int msgCheck = data_check(read);
				/* call the function to put the correct tag into pipe */
				if(msgCheck != -1)
					 put_monitor_data(read);
			}
		}
		if(on == 0)
		{
			ConnectDetach(conId);
			break;
		}
	}
	fclose(fp);
	return 0;
}

void* data_storage(void *a)
{
	/* function called by the write thread */
	while(true)
	{
		/* calling the function to read from the pipe */
		write_monitor_data();
		if(on == 0)
			break;
	}
	return 0;
}
/* End of thread functions */

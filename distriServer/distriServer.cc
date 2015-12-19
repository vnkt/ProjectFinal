/*
 *  Server side functionality implemented using pipes
 *
 *  Created on :
 *  Author :
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <sys/neutrino.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

#include "monitor_msg.h"
#include "dp566var.h"
#include "package.h"
#include "extern.hpp"
#include "threadfunc.h"

int config_count = 0;
int c = 0;
int on = 0;
int writeGlobal;
int fd[2];
char globalFileName[40];

FILE *globalfile = NULL;
name_attach_t *named_server;
Package configuration[10];
current_status_reply Current;

/* Signal handlers */
void onKill(int signal)
{
	printf("\n Signal kill Received \n ");
	fflush(stdout);
	close(fd[0]);
	close(fd[1]);
	if(writeGlobal == 1)
		fclose(globalfile);
	exit(1);
}
/* End of Signal handlers */


int main(int argc, char *argv[])
{
	pthread_t readThread;
	pthread_t writeThread;
	int recvId,suc,j;

	pthread_attr_t attr;
	pthread_attr_init(&attr );
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED );

	named_server = name_attach(NULL,SERVER,NAME_FLAG_ATTACH_GLOBAL);   /* creating channel for server - client communication */
	if(named_server == NULL)
	{
		printf("\nNamed server Creation Failed");
		exit(0);
	}

	signal( SIGKILL, onKill );   /* set own handler for signal kill and term */
	signal( SIGTERM, onKill );

	while(true)
	{
		monitor_buff input,reply;
		/* Receiving message from the client */
		recvId = MsgReceive(named_server->chid,&input,sizeof(input),NULL);
		if(recvId == -1)
			printf("\nReceive failed \n");
		if (input.pulse.type == _IO_CONNECT )
		{
			input.pulse.type = NULL;
			MsgReply( recvId, EOK, NULL, 0 );
			continue;
		}
		else if(recvId == 0)
		{
			/* Signal received  */
		}
		else
		{
			monitor_package_details_t temp;
			switch(input.message.msgType)
			{
				case CONFIG_MSG:
					/* configuration message from client to create objects */
					for(j = 0 ; j < input.message.configMsg.num_pkg; j++)
					{
						temp = input.message.configMsg.pkg_details[j];
						configuration[config_count].Init(temp);
						config_count++;
					}
					if(input.message.configMsg.store_full_bus_data == 1)
					{
						/* bus data write needed */
						writeGlobal = 1;
						strcpy(globalFileName, input.message.configMsg.bus_data_filename);
						globalfile = fopen(globalFileName,"w");
					}
					/* setting the reply for the configuration message */
					reply.message.msgType = CONFIG_REPLY;
					reply.message.configReply.objectNumber = config_count;
					reply.message.configReply.status = 1;
					suc = MsgReply(recvId,0,&reply,sizeof(monitor_buff));
					if(suc < 0)
					{
						printf("\nReply couldn't be sent ");
						exit(0);
					}
					break;

				case START_MSG:
					/* starting the data receive from the bus */
					if(on == 0 && config_count > 0)
					{
						on = 1;
						reply.message.startReply.status = 1;
						init_monitor_memory();
						/* creating the threads for data acquisition and data storage */
						pthread_create(&readThread, &attr , &data_acquisition, NULL);
						pthread_create(&writeThread, &attr , &data_storage, NULL);
						/* re - setting the counters for the data bus */
						Current.noOfInvalidMessages = 0;
						Current.noOfMessages = 0;
						Current.noOfValidMessages = 0;
						Current.noRTAlive = 0;
						for(int i = 0; i < 32; i++)
							for(int j = 0; j < 32 ; j++)
							{
								Current.rtSubAddrMessageCount[i][j] = -1;
								Current.rtInvalidSubAddrMessageCount[i][j] = -1;
							}
					}
					else if(config_count == 0)
						reply.message.startReply.status = -1;
					else
						reply.message.startReply.status = 0;
					reply.message.msgType = START_REPLY;
					suc = MsgReply(recvId,0,&reply,sizeof(monitor_buff));
					if(suc < 0)
					{
						printf("\nReply couldn't be sent ");
						exit(0);
					}
					break;

				case START_WO_REQ:
						/* starting the data receive from the bus */
						if(on == 0)
						{
							on = 1;
							reply.message.startReply.status = 1;
							init_monitor_memory();
							/* creating the threads for data acquisition and data storage */
							pthread_create(&readThread, &attr, &data_acquisition, NULL);
							pthread_create(&writeThread, &attr, &data_storage, NULL);
							/* re - setting the counters for the data bus */
							Current.noOfInvalidMessages = 0;
							Current.noOfMessages = 0;
							Current.noOfValidMessages = 0;
							Current.noRTAlive = 0;
							for(int i = 0; i < 32; i++)
								for(int j = 0; j < 32 ; j++)
								{
									Current.rtSubAddrMessageCount[i][j] = -1;
									Current.rtInvalidSubAddrMessageCount[i][j] = -1;
								}
							if(input.message.startWoMsg.writeNeeded == 1)
							{
								writeGlobal = 1;
								strcpy(globalFileName, input.message.startWoMsg.filename);
								globalfile = fopen(globalFileName,"w");
							}
							else
								writeGlobal = 0;
						}
						else
							reply.message.startReply.status = 0;
						reply.message.msgType = START_REPLY;
						suc = MsgReply(recvId,0,&reply,sizeof(monitor_buff));
						if(suc < 0)
						{
							printf("\nReply couldn't be sent ");
							exit(0);
						}
						break;

				case STOP_MSG:
					/* stopping the data receive from the bus */
					if(on == 1)
					{
						/* Stopping the two threads and resetting the pthread_t objects used */
						on = 0;
						pthread_cancel(readThread);
						pthread_cancel(writeThread);
						readThread = -1;
						writeThread = -1;
						reply.message.stopReply.status = 1;
						/* resetting all the global variables */
						config_count = 0;
						writeGlobal = 0;
						/* closing the pipe */
						close(fd[0]);
						close(fd[1]);
					}
					else
						reply.message.stopReply.status = 0;
					reply.message.msgType = STOP_REPLY;
					suc = MsgReply(recvId,0,&reply,sizeof(monitor_buff));
					if(suc < 0)
					{
						printf("\nReply couldn't be sent ");
						exit(0);
					}
					break;

				case REAL_REQ:
					/* request for real time data */
					real_reply realTime;
					if(input.message.realReq.objNumber > config_count) /* invalid request */
						realTime.status = -1;
					else
					{
						if(configuration[input.message.realReq.objNumber].data_rdy == 1)
						{
							/* getting the global buffer from the object and storing into the reply object */
							configuration[input.message.realReq.objNumber].getRealTime(realTime);
							realTime.status = 1;
						}
						else
							realTime.status  = 2;
					}
					suc = MsgReply(recvId,0,&realTime,sizeof(real_reply));
					if(suc < 0)
					{
						printf("\nReply couldn't be sent ");
						exit(0);
					}
					break;

				case CURR_REQ:
					/* request for real time data */
					Current.status = 1;
					if(writeGlobal == 1)
					{
						struct stat fileInfo;
						fstat( fileno(globalfile), &fileInfo );
						realTime.fileSize = fileInfo.st_size;
						Current.fileSize = realTime.fileSize;
					}
					Current.msgType = CURR_REPLY;
					suc = MsgReply(recvId,0,&Current,sizeof(current_status_reply));
					if(suc < 0)
					{
						printf("\nReply couldn't be sent ");
						exit(0);
					}
					break;
			}
		}
	}
}

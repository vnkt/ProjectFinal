/*
 * extern.hpp
 *
 *  Created on: Feb 17, 2012
 *      Author: user
 */

#ifndef EXTERN_HPP_
#define EXTERN_HPP_
#include "stdio.h"
#include "monitor_msg.h"
#include "package.h"
#include "sys/dispatch.h"

extern int config_count;
extern int c;
extern int on;
extern int writeGlobal;
extern int fd[2];
extern char globalFileName[40];

extern int channelId;
extern int conId;

extern FILE *globalfile;
extern name_attach_t *named_server;
extern Package configuration[10];
extern current_status_reply Current;


#endif /* EXTERN_HPP_ */

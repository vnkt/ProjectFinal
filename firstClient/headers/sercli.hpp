/*
 * sercli.hpp
 *
 *  Created on: Jan 30, 2012
 *      Author: user
 */
#ifndef _SERCLI_HPP
#define _SERCLI_HPP
typedef unsigned char byte;

#include "monitor_msg.h"

#define MAX_ERRORS  10

class MonClient
{
	private:
		const static char  *errmessg[MAX_ERRORS];
		monitor_buff msg, reply;
		pid_t coid;
		pid_t connect();
		int ToSvr();

	public:
		MonClient() {}
		~MonClient() {}
		int Init();

		int addconfig(monitor_package_details_t config);
		int setupstartwo(char *file, int size, int needed);
		int setupconfig();
		int setupstart();
		int setupstop();
		int setupreal(int objectNumber);
		int setupcurrent();
		int test();

		int outputbuf(byte *in);
		current_status_reply currentData;
		real_reply realtimeData;
};
#endif

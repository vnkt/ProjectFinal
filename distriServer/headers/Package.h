/*
 * Package.h
 *
 *  Created on: Jan 3, 2012
 *      Author: user
 */

#ifndef PACKAGE_H_
#define PACKAGE_H_

#include <monitor_msg.h>
#include <stdio.h>
#include <dp566var.h>

class Package
{
	private:
		monitor_package_details_t pkg;
		tagMSGRESULT curr_msg;

		short prev_msg_index;
		unsigned short minor_buffsize;
		unsigned char *minor_buf_ptr;
		unsigned char *major_buf_ptr;
		unsigned char minor_count;
		unsigned char msg_sts;

		unsigned char Init_buffer();
		short Get_Msg_Index();
		void Put_Minor_Data(unsigned short msg_index);
		unsigned char Put_Major_Data();
		unsigned char Write_Minor_Data();

	public:

		FILE *fp;
		int status;
		unsigned short error_status;    //to be passed to client to indicate error
		unsigned short data_rdy;
		unsigned short major_buffsize;
		unsigned char *global_buf_ptr;
		char Error_Msg[40];

		unsigned char Init(monitor_package_details_t pkg_in);
		void getRealTime(real_reply &reply);
		Package();
		 ~Package();
		unsigned char Proc_Msg(tagMSGRESULT msg);
};

#endif /* PACKAGE_H_ */

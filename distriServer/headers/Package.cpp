/*
 * Package.cpp
 *
 *  Created on: Jan 3, 2012
 *      Author: user
 */

#include "Package.h"
#include <stdio.h>
#include <string.h>

Package::Package()
{
	status = -1;
}

unsigned char Package::Init(monitor_package_details_t pkg_in)
{
	pkg = pkg_in;
	//initialize all variables
	minor_buf_ptr = major_buf_ptr = global_buf_ptr = NULL;
	fp = NULL;
	minor_count = 0;
	prev_msg_index = -1;
	msg_sts = 0;
	status = 0;

	if(Init_buffer())
	{
		status = -1;
		return 1;
	}
	if (!pkg.store_data)
		return 0;
	fp = fopen(pkg.filename,"w");
	if (fp == NULL)
		status = -1;
	return 1;
}

Package::~Package()
{
	Write_Minor_Data();

	if (fp != NULL)
		fclose(fp);
	if (minor_buf_ptr)
		delete[] minor_buf_ptr;
	if(major_buf_ptr)
		delete[] major_buf_ptr;
	if (global_buf_ptr)
		delete[] global_buf_ptr;

	minor_buf_ptr = major_buf_ptr = global_buf_ptr = NULL;
	return;
}

unsigned char Package::Init_buffer()
{
	//initialize minor and major data buffer
	unsigned short buf_size=0,i;
	if (pkg.frame_enclose)
		buf_size += 2;   //start and end tags
	if(pkg.insert_dummy_chars)
		buf_size += pkg.num_dummy_chars;
	for (i = 0;i < pkg.total_msg;i++)
		buf_size += pkg.word_count[i] * 2;  //one word = 2bytes

	minor_buffsize = buf_size;
	major_buffsize = minor_buffsize * pkg.total_minor_cycles;

	minor_buf_ptr = new unsigned char[minor_buffsize];
	if(minor_buf_ptr == NULL)
		return 1;
	//clear the buffer
	memset(minor_buf_ptr,0x00,minor_buffsize);

	major_buf_ptr = new unsigned char[major_buffsize];
	if(major_buf_ptr == NULL)
		return 1;
	//clear the buffer
	memset(major_buf_ptr,0x00,major_buffsize);

	global_buf_ptr = new unsigned char[major_buffsize];
	if(global_buf_ptr == NULL)
		return 1;
			//clear the buffer
	memset(global_buf_ptr,0x00,major_buffsize);
	return 0;

}

short Package::Get_Msg_Index()
{
	short i,index=-1;
	tagCMDWRD cmdWord;
	cmdWord =  *reinterpret_cast<tagCMDWRD*>(&curr_msg.usCmdWrd);

	if(cmdWord.RTAddress == pkg.RT_No)
	{
		for (i=0;i < pkg.total_msg ;i++)
		 {
			if ((cmdWord.SubAddr_ModeCode == pkg.Subaddress[i]) && (cmdWord.WrdCnt_ModeCode == pkg.word_count[i]))
			{
				index = i;
				break;
			}
		 }
	}
	else
		index = -1;

	return index;
}

unsigned char Package::Write_Minor_Data()
{
	if (status == -1)
		return 1;
	//do data framing
	if(pkg.frame_enclose)
	{
		*minor_buf_ptr = pkg.frame_start_char;
		*(minor_buf_ptr + minor_buffsize -1) = pkg.frame_end_char;
	}
	if(pkg.store_data)
	{
		if (fp == NULL)
			return 1;
		if(fwrite(minor_buf_ptr, sizeof(char), minor_buffsize,fp) != minor_buffsize)
			return 1;
	}
	return 0;
}

void Package::Put_Minor_Data(unsigned short  msg_index)
{
	short offset=0,i;
	if ((msg_index <= prev_msg_index) && (prev_msg_index != -1) ) //minor cycle boundary, ie one minor cycle is over
	{															//skip if it is the very first message (prev_msg_index = -1)
		Write_Minor_Data();  //write previous minor data
		Put_Major_Data();    //put previous minor data to major buffer
		//clear the minor buffer
		memset(minor_buf_ptr, 0x00, minor_buffsize);
		msg_sts = 0;
	}
	//put curr msg to current minor buffer
	// find the offset address in the minor data buffer to put the data in the correct slot
	if(pkg.frame_enclose)
		offset += 1;   //start char at the beginning
	if(pkg.insert_dummy_chars)
		offset += pkg.num_dummy_chars;  //additional dummy chars

	for (i=0;i<msg_index;i++)
		offset += ( pkg.word_count[i] * 2);

	//copy the current message data to minor buffer
	memcpy(minor_buf_ptr + offset, curr_msg.usData, pkg.word_count[msg_index]*2);

	//fill the msg status char
	msg_sts |= (0x1 << msg_index);  //set the msg status mask
	//put msg_sts if dummy chars enabled
	if(pkg.insert_dummy_chars)
		*(minor_buf_ptr + pkg.msgsts_word_pos - 1) = msg_sts;

	prev_msg_index = msg_index;
}

unsigned char Package::Put_Major_Data()
{
	short minor_id,offset=0;
	minor_count++;  //locally kept minor id

	if(pkg.total_minor_cycles > 1) //if major cycle enabled
	{
	//get the minor id from the existing minor buffer
	minor_id = *(unsigned char*)(minor_buf_ptr + pkg.minid_pos -1 );

	 if((minor_id < 1) ||(minor_id > pkg.total_minor_cycles)) //wild value at min_id position?
		error_status =1;
	 else
	 {
		minor_count = minor_id; //assume data buffer contains correct minor id, so use it for finding offset
		error_status = 0;
	 }
	}

	//put the minor data into correct major cycle slot
	//find the major buffer offset
	offset = minor_buffsize * (minor_count-1);
	memcpy(major_buf_ptr+offset,minor_buf_ptr,minor_buffsize);

	//if a major cycle is full, post it to global memory
	if (minor_count == pkg.total_minor_cycles)
	{
		//protect here with mutex???
		memcpy(global_buf_ptr,major_buf_ptr,major_buffsize);
		data_rdy =1;
		//reset minor count
		minor_count =0;
		//clear major buffer
		memset(major_buf_ptr,0x00,major_buffsize);
	}

	return 0;
}


void Package::getRealTime(real_reply &reply)
{
	memcpy(reply.reply,global_buf_ptr,major_buffsize);
	reply.count = major_buffsize;
}

unsigned char Package::Proc_Msg(tagMSGRESULT msg)
{
	short msg_index;
	curr_msg = msg;

	msg_index = Get_Msg_Index();
	if (msg_index == -1)
		return 0;
	else
		Put_Minor_Data (msg_index);
	return 0;
}

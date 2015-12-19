/*
 * DbConnect.cpp
 *
 *  Created on: Feb 7, 2012
 *      Author: user
 */

#include "sqlitewrapped.h"
#include "sqliteDB.hpp"
#include "DbConnect.hpp"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define HUGE_VAL	(__builtin_huge_val())

template<class _Ty> inline
	_Ty _Pow_int(_Ty _Left, int _Right)
	{	// raise to integer power
	unsigned int _Num = _Right;
	if (_Right < 0)
		_Num = 0 - _Num;

	for (_Ty _Ans = 1; ; _Left *= _Left)
		{	// scale and fold in factors
		if ((_Num & 1) != 0)
			_Ans *= _Left;
		if ((_Num >>= 1) == 0)
			return ((_Ty)(0 <= _Right ? _Ans
				: _Ans == _Ty(0) ? HUGE_VAL : _Ty(1) / _Ans));
	}
}

packageConnect::packageConnect()
{
	selected_count = 0;
	available_count = 0;
}

packageConnect::~packageConnect()
{
	if (real_time_buffer)
		delete[] real_time_buffer;
}

void packageConnect::Init(char *pack, char *file)
{
	strcpy(pack_type, pack);
	strcpy(file_name, file);
	strcpy(details.filename, file);
	if(strcmp(file,"") == 0)
		details.store_data = 0;
	else
		details.store_data = 1;
	connector.Init("/home/btp/dbname");

	strcpy(query,"select * from mformat where PACKAGE = \"");
	strcat(query, pack);
	strcat(query, "\"");
	connector.get_result(query);
	while (connector.fetch_row())
	{
		min_bytes = connector.getval("MI_BYTES");
		details.RT_No = connector.getval("RT_NO");
		details.total_minor_cycles = connector.getval("MIN_FRAMES");
		details.minid_pos = connector.getval("ID_POS");
		connector.getstr(package_det_table,"MSG_DET");
		connector.getstr(package_fields_table,"FMT_FILE");
		details.frame_start_char = connector.getval("START_FRAME");
		details.frame_end_char = connector.getval("END_FRAME");
		if(details.frame_start_char == NULL)
			details.frame_enclose = 0;
		else
			details.frame_enclose = 1;
		details.num_dummy_chars = connector.getval("NO_DUMMY_CHAR");
		if(details.num_dummy_chars == 0)
				details.insert_dummy_chars = 0;
		else
		{
			details.insert_dummy_chars = 1;
			details.msgsts_word_pos = connector.getval("STATUS_POS");
		}
	}
	connector.free_result();

	SqliteDB temp;
	temp.Init("/home/btp/dbname");
  	details.total_msg = 0;
	sprintf(query,"select * from %s", package_det_table);
	temp.get_result(query);
	while (temp.fetch_row())
	{
		details.Subaddress[details.total_msg] = temp.getval("SUB_ADDR");
		details.word_count[details.total_msg] = temp.getval("WORD_COUNT");
		details.total_msg++;
	}
	temp.free_result();
	fetch_available_groups();
	available_field_count();
	add_field("rtc");
}

void packageConnect::get_avaiable_field(char *output, int position)
{
	if(position > available_count)
		return;
	else
	{
		sprintf(query,"select * from %s where ROWID = %d", package_fields_table, position);
		connector.get_result(query);
		while(connector.fetch_row())
		{
			if(connector.getval("PTYPE") == FLOATING || connector.getval("PTYPE") == INTEGER || connector.getval("PTYPE") == COUNT || connector.getval("PTYPE") == FLAG)
				connector.getstr(output, "STR_SIGLEN");
			else
				strcpy(output, "NULL");
		}
	}
	connector.free_result();
}

int packageConnect::get_avaiable_field(char **output)
{
	int x = 0;
	sprintf(query,"select STR_SIGLEN from %s", package_fields_table);
	connector.get_result(query);
	while(connector.fetch_row())
	{
		connector.getstr(output[x]);
		x ++;
	}
	connector.free_result();
	return x;
}

int packageConnect::add_field(char *field)
{
	int flag = 0;
	if(strcmp(field, "") == 0)
		return -1;
	for(int i = 0; i < selected_count; i++ )
	{
		if(strcmp(fields[i],field) == 0)
		{
			flag = 1;
			break;
		}
	}
	if(flag == 1)
		return -1;

	strcpy(fields[selected_count], field);
	selected_count++;

	return 0;
}

int packageConnect::add_field(int index)
{
	int flag = 0;
	char field[15];

	sprintf(query,"select STR_SIGLEN from %s where ROWID = %d", package_fields_table, index);
	connector.get_result(query);
	while(connector.fetch_row())
		connector.getstr(field);
	connector.free_result();

	if(strcmp(field, "") == 0)
		return -1;
	for(int i = 0; i < selected_count; i++ )
	{
		if(strcmp(fields[i],field) == 0)
		{
			flag = 1;
			break;
		}
	}
	if(flag == 1)
		return -1;

	strcpy(fields[selected_count], field);
	selected_count++;
	return 0;
}

int packageConnect::remove_field(char *field)
{
	for(int i = 0; i < selected_count; i++ )
	{
		if(strcmp(fields[i], field) == 0)
		{
			for(int j = i; j < selected_count; j++,i++)
				strcpy(fields[j],fields[j + 1]);
		}
	}
	selected_count--;
	if(selected_count < 0)
			return -1;
	else
		return 0;
}

int packageConnect::remove_field(int pos)
{
	for(int j = pos; j < selected_count; j++)
		strcpy(fields[j],fields[j + 1]);
	selected_count--;
	if(selected_count < 0)
		return -1;
	else
		return 0;
}

monitor_package_details_t packageConnect::get_config()
{
	return details;
}

int packageConnect::get_selected_field_count()
{
	return selected_count;
}

void packageConnect::get_pack_field(char *ouptut, int position)
{
	strcpy(ouptut, fields[position]);
}

int packageConnect::get_realtime_value(int pos, char *out_value, char *out_name, char *out_unit)
{
	double value;
	int status = 0, readpos = 0;
	unsigned char input[5];

	sprintf(query,"select * from %s where STR_SIGLEN = \"%s\"", package_fields_table, fields[pos]);
	connector.get_result(query);
	while(connector.fetch_row())
	{
		strcpy(out_name, fields[pos]);
		if(connector.getval("ID") != 0)
			readpos = (min_bytes * ( connector.getval("ID") - 1 )) + connector.getval("POSITION") - 1;
		else
			readpos = (12 * min_bytes) + connector.getval("POSITION") - 1;
		memcpy(input, real_time_buffer + readpos,connector.getval("NB"));
		connector.getstr(out_unit,"UNI_SIGLEN");
		value = convert(input, connector.getval("NB"), connector.getnum("SF"), connector.getval("PTYPE"), connector.getnum("NF"), out_unit);
		if(connector.getval("PTYPE") == 0)
			sprintf(out_value, "%+.4f",value);
		else if(connector.getval("PTYPE") == 1 || connector.getval("PTYPE") == 2)
		{
			int va = value;
			sprintf(out_value, "+%d",va);
		}
		else if(connector.getval("PTYPE") == 3)
		{
			int va = value;
			sprintf(out_value, "%x",va);
		}
		if(connector.getnum("MIN_VAL") == 0 && connector.getnum("MAX_VAL") == 0)
			status = 0;
		else if(value < connector.getnum("MIN_VAL"))
			status = -1;
		else if(value > connector.getnum("MAX_VAL"))
			status = 1;
	}
	connector.free_result();
	return status;
}

int packageConnect::get_realtime_value(int pos, char *out_name, char *out_unit)
{
	sprintf(query,"select * from %s where STR_SIGLEN = \"%s\"", package_fields_table, fields[pos]);
	connector.get_result(query);
	while(connector.fetch_row())
	{
		strcpy(out_name, fields[pos]);
		connector.getstr(out_unit,"UNI_SIGLEN");
	}
	connector.free_result();
	return 0;
}

void packageConnect::set_RT_buffer(unsigned char *input, int len)
{
	real_time_buffer = new unsigned char[len];
	memcpy(real_time_buffer, input, len);
}

void packageConnect::fetch_available_groups()
{
	group_count = 0;
	sprintf(query, "select DISTINCT GROUP_NAME from %s", package_fields_table);
	connector.get_result(query);
	while (connector.fetch_row())
	{
		connector.getstr(available_groups[group_count]);
		group_count++;
	}
	connector.free_result();
}

int packageConnect::add_group(char *group)
{
	char field_name[25];
	sprintf(query, "select * from %s where GROUP_NAME = \"%s\"", package_fields_table, group);
	connector.get_result(query);
	while (connector.fetch_row())
	{
		if(connector.getval("PTYPE") == FLOATING || connector.getval("PTYPE") == INTEGER || connector.getval("PTYPE") == COUNT || connector.getval("PTYPE") == FLAG)
		{
			connector.getstr(field_name, "STR_SIGLEN");
			add_field(field_name);
		}
	}
	connector.free_result();
	return 0;
}

void packageConnect::available_field_count()
{
	sprintf(query,"select count(*) from %s", package_fields_table);
	connector.get_result(query);
	while (connector.fetch_row())
	{
		available_count = connector.getval();
	}
	connector.free_result();
}

int packageConnect::get_available_field_count()
{
	return available_count;
}

int packageConnect::change_group(char *field, char *group_name)
{
	sprintf(query, "update %s set GROUP_NAME = \"%s\" where STR_SIGLEN = \"%s\"", package_fields_table, group_name, field);
	return connector.execute(query);
}

void packageConnect::clear()
{
	selected_count = 0;
	available_count = 0;
	group_count = 0;
	strcpy(file_name, "");
}


/*
 * End of packageConnect functions and
 * start of
 * Conversion functions
 *
 */
float packageConnect::convert(unsigned char *input, int NB, float scale, int ptype, float nf, char *unit)
{
	float return_value = 0;
	long value;
	value = get_long(input, NB);

	switch(ptype)
	{
		case FLOATING:
				return_value = convert_to_float(value, nf, scale);
				break;
		case INTEGER:
				return_value = convert_to_integer(value, nf, scale);
				break;
		case COUNT:
				return_value = convert_to_count(value, nf, scale);
				break;
		case FLAG:
				return_value = convert_to_flag(value, nf, scale);
				break;
	}
	return return_value;
}

double packageConnect::convert_to_float(long value, float normal, float scale)
{
	double output = 1;
	if(normal == 0.0)
		output = value * scale;

	else if(normal == 1.0)
	{
		if(value > 32767)
			output = ( -1 ) * ((value * scale) / pow(2,16));
		else
			output = ((value * scale) / pow(2,15));
	}
	return output;
}

long packageConnect::convert_to_count(long  value, float normal, float scale)
{
	long output = 1;
	if(normal == 0.0)
		output = value * scale;
	return output;
}

long packageConnect::convert_to_flag(long  value, float normal, float scale)
{
	return value;
}

int packageConnect::convert_to_integer(long  value, float normal, float scale)
{
	int output = 1;
	if(normal == 0.0)
		output = value * scale;

	return output;
}

double packageConnect::pow(double _Left, int _Right)
{
	return (_Pow_int(_Left, _Right));
}

long packageConnect::get_long(unsigned char *input, int num_bytes)
{
	unsigned long num = 0;
	for(int i = num_bytes - 1; i >= 0; i--)
	{
		num |= input[i];
		if( i != 0)
			num = num << 8;
	}
	return num;
}

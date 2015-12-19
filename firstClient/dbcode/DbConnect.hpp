/*
 * DbConnect.hpp
 *
 *  Created on: Feb 7, 2012
 *      Author: user
 */

#ifndef DBCONNECT_HPP_
#define DBCONNECT_HPP_

#include "sqlitewrapped.h"
#include "sqliteDB.hpp"
#include "monitor_msg.h"

#define FLAG      	3
#define COUNT     	2
#define INTEGER   	1
#define FLOATING  	0

class packageConnect
{
	private:
		SqliteDB connector;
		char query[60];
		char fields[50][15];
		int available_count;
		int selected_count;
		char package_det_table[30];
		char package_fields_table[30];
		unsigned char *real_time_buffer;
		monitor_package_details_t details;
		int min_bytes;

		double convert_to_float(long value, float normal , float scale);
		int convert_to_integer(long value, float normal , float scale);
		long convert_to_count(long value, float normal , float scale);
		long convert_to_flag(long value, float normal , float scale);
		long get_long(unsigned char *c, int num_bytes);
		void available_field_count();

	public:
		char available_groups[30][15];
		int group_count;
		char file_name[15];
		char pack_type[10];
		packageConnect();
		~packageConnect();

		void Init(char *pack, char *file);
		void get_avaiable_field(char *ouptut, int position);
		int get_avaiable_field(char **ouptut);
		void get_pack_field(char *ouptut,int position);

		int add_field(char *field);
		int add_field(int index);
		int add_group(char *group);
		int remove_field(char *field);
		int remove_field(int pos);

		monitor_package_details_t get_config();

		int get_selected_field_count();
		int get_available_field_count();
		int change_group(char *field, char * group_name);
		void fetch_available_groups();

		void set_RT_buffer(unsigned char *input, int len);
		int get_realtime_value(int pos, char *out_value, char *out_name, char *out_unit);
		int get_realtime_value(int pos, char *out_name, char *out_unit);
		void clear();

		float convert(unsigned char *input, int NB, float scale, int ptype, float nf, char *unit);
		double pow(double _Left, int _Right);
};
#endif /* DBCONNECT_HPP_ */

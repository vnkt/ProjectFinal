/*
 * threadfunc.h
 *
 *  Created on: Feb 21, 2012
 *      Author: user
 */

#ifndef THREADFUNC_H_
#define THREADFUNC_H_
#include "dp566var.h"

int data_check(tagMSGRESULT &tag);
void write_monitor_data();
int put_monitor_data(tagMSGRESULT &tag);
void* data_acquisition(void *a);
void* data_storage(void *a);

int init_monitor_memory();
int close_monitor_memory();
void setUpTimer();

#endif /* THREADFUNC_H_ */

/* Y o u r   D e s c r i p t i o n                       */
/*                            AppBuilder Photon Code Lib */
/*                                         Version 2.03  */

/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* Local headers */
#include "ablibs.h"
#include "abimport.h"
#include "proto.h"
#include "DbConnect.hpp"
#include "sercli.hpp"
#include "common_utils.hpp"

packageConnect db[10];
char packages[3][10] = {"NIM", "i960-F06", "Vik_F06"};
char selected[3];
char selected_group[20][15];
int pack_count = 0, group_count = 0;
int selectedPackage;
MonClient sendToServer;
bool realOn = false;
bool graph = false;
FILE *fp;

int add_package_callback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	unsigned short *ind;
	char *fileName;

	PtGetResource(ABW_rtana_package_select_combo,Pt_ARG_CBOX_SEL_ITEM,&ind,0);
	if( *ind == 0 )
		return 0;
	for(int i = 0; i < pack_count; i++)
		if(selected[i] == (*ind - 1))
			return (Pt_CONTINUE);
	selected[pack_count] = *ind - 1;
	const char* temp[] = {packages[*ind - 1]};
	PtListAddItems(ABW_rtana_selected_package_list,temp,1,0);
	PtGetResource(ABW_rtana_file_name_text, Pt_ARG_TEXT_STRING, &fileName, 0 );
	PtSetResource(ABW_rtana_file_name_text, Pt_ARG_TEXT_STRING, "", 0 );
	db[pack_count++].Init(packages[*ind - 1], fileName);

	return(Pt_CONTINUE);
}

int add_field_callback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	unsigned short *ind_field, *ind_group;
	char already_selected[20];

	PtGetResource(ABW_rtana_available_fields_list,Pt_ARG_SELECTION_INDEXES,&ind_field,0);
	PtGetResource(ABW_rtana_available_groups_list,Pt_ARG_SELECTION_INDEXES,&ind_group,0);

	if((*ind_field == 0 || *ind_field  > db[selectedPackage].get_available_field_count()) && (*ind_group == 0 || *ind_group > db[selectedPackage].group_count))
		return (Pt_CONTINUE);

	if((*ind_group != 0 && *ind_group - 1 < db[selectedPackage].group_count))
	{
		db[selectedPackage].add_group(db[selectedPackage].available_groups[*ind_group - 1]);
		PtListSelectPos(ABW_rtana_available_groups_list,*ind_group + 1 );
		PtListShowPos(ABW_rtana_available_groups_list,*ind_group + 1 );
	}
	else if((*ind_field != 0 && *ind_field -1  < db[selectedPackage].get_available_field_count()))
	{
		PtArg_t args[1];
		short *num;
		char **items = NULL;

		PtSetArg(&args[0], Pt_ARG_ITEMS, &items, &num);
		PtGetResources(ABW_rtana_available_fields_list, 1, args);

		db[selectedPackage].add_field(items[*ind_field - 1]);
		PtListSelectPos(ABW_rtana_available_fields_list,*ind_field + 1 );
		PtListShowPos(ABW_rtana_available_fields_list,*ind_field + 1);
	}
	PtListDeleteAllItems(ABW_rtana_selected_fields_list);
	int ret = db[selectedPackage].get_selected_field_count();
	for(int i = 0; i < ret; i++)
	{
		db[selectedPackage].get_pack_field(already_selected, i);
		const char* temp[] = {already_selected};
		PtListAddItems(ABW_rtana_selected_fields_list,temp,1,0);
	}
	PtSetResource(ABW_rtana_remove_field_button,Pt_ARG_FLAGS,Pt_FALSE,Pt_BLOCKED|Pt_GHOST);

	return(Pt_CONTINUE);
}

int remove_field_callback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	unsigned short *ind;
	PtGetResource(ABW_rtana_selected_fields_list,Pt_ARG_SELECTION_INDEXES,&ind,0);
	if(*ind == 0 || *ind > db[selectedPackage].get_selected_field_count())
		return (Pt_CONTINUE);
	int avail = db[selectedPackage].remove_field(*ind - 1);
	PtListDeleteItemPos(ABW_rtana_selected_fields_list, 1,*ind);
	if(avail == -1)
		PtSetResource(ABW_rtana_remove_field_button,Pt_ARG_FLAGS,Pt_TRUE,Pt_BLOCKED|Pt_GHOST);

	return(Pt_CONTINUE);
}

int display_fields_callback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	unsigned short *ind;
	char already_selected[20];
	char available_field[20];
	int i;
	int selection_count = db[selectedPackage].get_selected_field_count();
	int avl_count = db[selectedPackage].get_available_field_count();

	PtSetResource(ABW_rtana_file_name_text, Pt_ARG_TEXT_STRING, db[selectedPackage].file_name, 0 );
	PtListDeleteAllItems(ABW_rtana_selected_fields_list);
	for(i = 0; i < selection_count; i++)
	{
		db[selectedPackage].get_pack_field(already_selected, i);
		const char* temp[] = {already_selected};
		PtListAddItems(ABW_rtana_selected_fields_list,temp,1,0);
	}
	PtGetResource(ABW_rtana_selected_package_list,Pt_ARG_SELECTION_INDEXES,&ind,0);
	selectedPackage = *ind - 1;
	PtListDeleteAllItems(ABW_rtana_available_fields_list);

	for(i = 1; i <= avl_count; i++)
	{
		db[selectedPackage].get_avaiable_field(available_field,i);
		if(strcmp(available_field, "NULL") != 0)
		{
			const char* temp[] = {available_field};
			PtListAddItems(ABW_rtana_available_fields_list,temp,1,0);
		}
	}

	PtListDeleteAllItems(ABW_rtana_available_groups_list);
	for(i = 0; i < db[selectedPackage].group_count; i++)
	{
		const char* temp[] = {db[selectedPackage].available_groups[i]};
		PtListAddItems(ABW_rtana_available_groups_list,temp,1,0);
	}

	return(Pt_CONTINUE);
}

int packana_stop_callback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	realOn = false;
	sendToServer.setupstop();
	PtSetResource(ABW_packana_stop_button,Pt_ARG_FLAGS,Pt_TRUE,Pt_BLOCKED|Pt_GHOST);

	return( Pt_CONTINUE );
}


int packana_post_realise_callback( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	realOn = true;
	graph = false;
	Panel_Display display_mult_pack,display_mult_field,display_mult_value,display_mult_unit ;
	display_mult_pack.Init(ABW_packana_display_pack_mltitext);
	display_mult_field.Init(ABW_packana_display_field_mltitext);
	display_mult_value.Init(ABW_packana_display_value_mltitext);
	display_mult_unit.Init(ABW_packana_display_unit_mltitext);

	unsigned char buffer[5000];
	char loop_name[25], loop_value[10], loop_unit[10];
	int buffer_size, row_no = 1;

	for(int i = 0; i < pack_count; i++)
		sendToServer.addconfig(db[i].get_config());
	sendToServer.setupconfig();
	sendToServer.setupstart();

	while(true)
	{
		for(int i = 0 ; i < pack_count; i++)
		{
			sendToServer.setupreal(i);
			buffer_size = sendToServer.outputbuf(buffer);
			if(buffer_size == 0)
				continue;
			db[i].set_RT_buffer(buffer, buffer_size);
			int field_count = db[i].get_selected_field_count();
			display_mult_pack.Clear();
			display_mult_field.Clear();
			display_mult_value.Clear();
			display_mult_unit.Clear();
			for(int j = 0; j < field_count; j++)
			{
				int ret_val = db[i].get_realtime_value(j,loop_value, loop_name, loop_unit);

				display_mult_pack.wgprintf(row_no, 1, Pg_BLACK, 0, 0, "%s",db[i].pack_type);
				display_mult_field.wgprintf(row_no, 1, Pg_BLACK, 0, 0, "%s",loop_name);
				if(ret_val == 1)
					display_mult_value.wgprintf(row_no, 1, Pg_RED, 0, 0 ,"%s",loop_value);
				else if(ret_val == -1)
					display_mult_value.wgprintf(row_no, 1, Pg_BLUE, 0, 0, "%s",loop_value);
				else
					display_mult_value.wgprintf(row_no, 1, Pg_BLACK, 0, 0, "%s",loop_value);
				display_mult_unit.wgprintf(row_no, 1, Pg_BLACK, 0, 0, "%s",loop_unit);

				row_no++;
				if(graph == true)
					fprintf(fp, "%-18s", loop_value);
			}
			if(graph == true)
				fprintf(fp,"\n");
		}
		row_no = 1;
		PtBkgdHandlerProcess();
		delay(500);
		if(realOn == false)
			break;
	}
	return( Pt_CONTINUE );
}

int rtana_realized_callback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	for(int i = 0 ; i < pack_count; i++)
		db[i].clear();
	pack_count = 0;
	return( Pt_CONTINUE );
}


int packana_window_close_callback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	realOn = false;
	sendToServer.setupstop();
	system("slay graphTrial");
	return( Pt_CONTINUE );
}


int newgrp_realised_callback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	int avl_count = db[selectedPackage].get_available_field_count();
	char available_field[50];

	for(int i = 1; i < avl_count; i++)
	{
		db[selectedPackage].get_avaiable_field( available_field, i);
		const char* temp[] = { available_field };
		PtListAddItems(ABW_newgrp_available_list,temp,1,0);
		PtBkgdHandlerProcess();
	}
	return( Pt_CONTINUE );
}


int add_fields_callback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	//selected_group;
	unsigned short *ind;

	PtGetResource(ABW_newgrp_available_list,Pt_ARG_SELECTION_INDEXES,&ind,0);
	if(*ind == 0)
		return Pt_CONTINUE;
	db[selectedPackage].get_avaiable_field(selected_group[group_count],*ind);
	const char* temp[] = { selected_group[group_count] };
	PtListAddItems(ABW_newgrp_selected_list,temp,1,0);
	group_count++;
	PtSetResource(ABW_newgrp_create_group_button,Pt_ARG_FLAGS,Pt_FALSE,Pt_BLOCKED|Pt_GHOST);
	return( Pt_CONTINUE );
}


int create_group_callback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	char *group_name;
	int i;

	PtGetResource(ABW_newgrp_name_text, Pt_ARG_TEXT_STRING, &group_name, 0 );
	for(i = 0; i < group_count; i++)
		db[selectedPackage].change_group(selected_group[i],group_name);

	PtListDeleteAllItems(ABW_rtana_available_groups_list);
	db[selectedPackage].fetch_available_groups();
	for(i = 0; i < db[selectedPackage].group_count; i++)
	{
		const char* temp[] = {db[selectedPackage].available_groups[i]};
		PtListAddItems(ABW_rtana_available_groups_list,temp,1,0);
	}
	PtDestroyWidget(ABW_new_group_window);
	return( Pt_CONTINUE );
}


int remove_fields_callback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	unsigned short *ind;
	PtGetResource(ABW_newgrp_available_list,Pt_ARG_SELECTION_INDEXES,&ind,0);
	if(*ind == 0)
		return Pt_CONTINUE;
	for(int i = *ind - 1; i < group_count; i++ )
		strcpy(selected_group[i], selected_group[i + 1]);
	group_count--;

	PtListDeleteAllItems(ABW_newgrp_selected_list);
	for(int i = 0; i < group_count; i++)
	{
		const char* temp[] = { selected_group[i] };
		PtListAddItems(ABW_newgrp_selected_list,temp,1,0);
	}
	return( Pt_CONTINUE );
}

int rtana_field_selected_callback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	unsigned short *ind;

	PtGetResource(ABW_rtana_available_groups_list,Pt_ARG_SELECTION_INDEXES,&ind,0);
	PtListUnselectPos( ABW_rtana_available_groups_list , *ind);
	return( Pt_CONTINUE );
}


int rtana_group_selected_callback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	unsigned short *ind;

	PtGetResource(ABW_rtana_available_fields_list,Pt_ARG_SELECTION_INDEXES,&ind,0);
	PtListUnselectPos( ABW_rtana_available_fields_list , *ind);
	return( Pt_CONTINUE );
}

int packan_ok_callback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	realOn = false;
	sendToServer.setupstop();
	PtDestroyWidget(ABW_package_analysis_window);
	return( Pt_CONTINUE );
}


int rtana_hover_package( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	PtGiveFocus( ABW_rtana_file_name_text, cbinfo->event );
	return( Pt_CONTINUE );
}

int rtgraph( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	char loop_name[25], loop_unit[25];
	system( "plot" );
	mkfifo("pump_rta", S_IWUSR);
	fp = fopen("pump_rta", "w");
	for(int i = 0; i < 9; i++)
		fprintf(fp, "\n");
	for(int i = 0 ; i < pack_count; i++)
	{
		int field_count = db[i].get_selected_field_count();
		for(int j = 0; j < field_count; j++)
		{
			db[i].get_realtime_value(j, loop_name, loop_unit);
			fprintf(fp, "%-18s", loop_name);
		}
	}
	fprintf(fp, "\n");
	for(int i = 0 ; i < pack_count; i++)
	{
		int field_count = db[i].get_selected_field_count();
		for(int j = 0; j < field_count; j++)
		{
			db[i].get_realtime_value(j, loop_name, loop_unit);
			fprintf(fp, "%-18s", loop_unit);
		}
	}
	fprintf(fp, "\n\n");
	graph = true;

	return( Pt_CONTINUE );
}


int rtana_cancel_callback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	PtDestroyWidget(ABW_real_time_analysis);
	return( Pt_CONTINUE );
}


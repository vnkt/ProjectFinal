/* Call backs from the application                     */
/*                            AppBuilder Photon Code Lib */
/*                                         Version 2.03  */

/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <Ap.h>

/* Local headers */
#include "ablibs.h"
#include "abimport.h"
#include "proto.h"
#include "sercli.hpp"

MonClient toServer;
bool acq_on = false;
PtFileSelectionInfo_t info;
PtArg_t args[1];

int dtacq_select_file_callback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* File select dialog box */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	/* Initialize the widget library and create a file dialog */
	PtInit( NULL );
	memset( &info, 0x0, sizeof(PtFileSelectionInfo_t) );
	info.args = args;
	info.num_args = 1;
	PtFileSelection( NULL, NULL, "File Selection", "~",  NULL, NULL, NULL, NULL, &info,	Pt_FSR_SHOW_HIDDEN | Pt_FSR_NO_FCHECK);
    if ( info.ret == Pt_FSDIALOG_BTN1 )
    {
    	PtSetResource(ABW_dtacq_file_select_text, Pt_ARG_TEXT_STRING, info.path,0); /* Set the file selected in the file selected text box */
    	PtSetResource(ABW_dtacq_data_analysis_button,Pt_ARG_FLAGS,Pt_FALSE,Pt_BLOCKED|Pt_GHOST);
    }
    else
    {
    	char an[1] = "";
    	PtSetResource(ABW_dtacq_file_select_text, Pt_ARG_TEXT_STRING, &an,0);
    	PtSetResource(ABW_dtacq_data_analysis_button,Pt_ARG_FLAGS,Pt_TRUE,Pt_BLOCKED|Pt_GHOST);
    }
    return( Pt_CONTINUE );
}


int dtacq_start_da_callback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	 /* On start data acquisition of data acquisition window */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	acq_on = true;
	char *filename;
	if(toServer.Init() == -1)
	{
		char Helvetica12[MAX_FONT_TAG];
		PtNotice( ABW_data_acquisition_window, NULL, "Warning", NULL,"Connection non existant",PfGenerateFontName("Helvetica", 0, 12, Helvetica12),NULL, NULL, Pt_BLOCK_PARENT|Pt_WAIT|Pt_RELATIVE|Pt_ESC_DISABLE );
		return Pt_CONTINUE;
	}
	PtSetResource(ABW_dtacq_stop_data_acquisition_button,Pt_ARG_FLAGS,Pt_FALSE,Pt_BLOCKED|Pt_GHOST);  /* Enable stop */
	PtSetResource(ABW_dtacq_start_data_acquisition_button,Pt_ARG_FLAGS,Pt_TRUE,Pt_BLOCKED|Pt_GHOST);  /* Disable start */
	PtSetResource(ABW_dtacq_data_analysis_button,Pt_ARG_FLAGS,Pt_TRUE,Pt_BLOCKED|Pt_GHOST); /* Disable data analysis */
	PtGetResource(ABW_dtacq_file_name_text, Pt_ARG_TEXT_STRING, &filename, 0 ); /* Check for the filename to store the data */
	if(strcmp(filename,"") != 0)
	{
		PtSetResource(ABW_dtacq_file_size_label,Pt_ARG_FLAGS,Pt_FALSE,Pt_BLOCKED|Pt_GHOST); /* Enable file size */
		PtSetResource(ABW_dtacq_file_size_display_label,Pt_ARG_FLAGS,Pt_FALSE,Pt_BLOCKED|Pt_GHOST);
		toServer.setupstartwo(filename, strlen(filename),1); /* Start command to the server with file name */
	}
	else
	{
		char Helvetica12[MAX_FONT_TAG];
		PtNotice( ABW_data_acquisition_window, NULL, "Warning", NULL,"Acquired data will not be stored!",PfGenerateFontName("Helvetica", 0, 12, Helvetica12),NULL, NULL, Pt_BLOCK_PARENT|Pt_WAIT|Pt_RELATIVE|Pt_ESC_DISABLE );
		toServer.setupstartwo(filename,0,0); /* Start command to the server without file name */
	}
	char a[2];
	strcpy(a," : NA");
	PtSetResource(ABW_dtacq_file_size_display_label, Pt_ARG_TEXT_STRING, &a ,0);
	while(true)
	{
		PtListDeleteAllItems(ABW_dtacq_rtno_list);
		PtListDeleteAllItems(ABW_dtacq_subaddress_list);
		PtListDeleteAllItems(ABW_dtacq_correct_count_list);
		PtListDeleteAllItems(ABW_dtacq_error_count_list);
		toServer.setupcurrent();
		for(int i = 0; i < 32; i++)
			for(int j= 0 ; j < 32; j++)
			{
				if(toServer.currentData.rtSubAddrMessageCount[i][j] != -1 || toServer.currentData.rtInvalidSubAddrMessageCount[i][j] != -1)
				{
					itoa(i,a,10);
					const char* temp1[] = {a};
					PtListAddItems(ABW_dtacq_rtno_list,temp1,1,0);

					itoa(j,a,10);
					const char* temp2[] = {a};
					PtListAddItems(ABW_dtacq_subaddress_list,temp2,1,0);

					itoa(toServer.currentData.rtSubAddrMessageCount[i][j]+1 + toServer.currentData.rtInvalidSubAddrMessageCount[i][j]+1,a,10);
					const char* temp3[] = {a};
					PtListAddItems(ABW_dtacq_correct_count_list,temp3,1,0);

					itoa(toServer.currentData.rtInvalidSubAddrMessageCount[i][j]+1,a,10);
					const char* temp4[] = {a};
					PtListAddItems(ABW_dtacq_error_count_list,temp4,1,0);

					if(strcmp(filename,"") != 0)
					{
						itoa(toServer.currentData.fileSize, a, 10);
						PtSetResource(ABW_dtacq_file_size_display_label, Pt_ARG_TEXT_STRING, &a ,0);
					}
				}
			}
		PtBkgdHandlerProcess();
		delay(500);
		if(acq_on == false)
				break;
	}
	return( Pt_CONTINUE );
}


int dtacq_stop_da_callback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* On stop data acquisition of data acquisition window */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	acq_on = false;
	PtSetResource( ABW_dtacq_stop_data_acquisition_button,Pt_ARG_FLAGS,Pt_TRUE,Pt_BLOCKED|Pt_GHOST); /* Disable stop */
	if(info.ret == Pt_FSDIALOG_BTN1)
			PtSetResource( ABW_dtacq_data_analysis_button,Pt_ARG_FLAGS,Pt_FALSE,Pt_BLOCKED|Pt_GHOST); /* Enable data analysis */
	PtSetResource( ABW_dtacq_start_data_acquisition_button,Pt_ARG_FLAGS,Pt_FALSE,Pt_BLOCKED|Pt_GHOST); /* Enable start */
	toServer.setupstop(); /* Stop command to the server */
	return( Pt_CONTINUE );
}

int init_start( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* At the beginning of execution - On start */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	/* Connecting to the server */
	toServer.Init();
	return( Pt_CONTINUE );
}

int base_test_connectivity_callback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	char status[20];
	if(toServer.Init() == -1)
	{
		strcpy(status, "Connection failed");
		PtSetResource(ABW_base_connec_status_label, Pt_ARG_COLOR, Pg_RED,0);
		PtSetResource(ABW_base_connec_status_label, Pt_ARG_TEXT_STRING, &status,0);
	}
	else
	{
		strcpy(status, "Connection established");
		PtSetResource(ABW_base_connec_status_label, Pt_ARG_COLOR, Pg_DGREEN,0);
		PtSetResource(ABW_base_connec_status_label, Pt_ARG_TEXT_STRING, &status,0);
	}
	return( Pt_CONTINUE );
}


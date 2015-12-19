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
#include "comboFunc.h"
#include "dp566var.h"

FILE *fin, *fout;
tagMSGRESULT buffer;
tagCMDWRD cmdWord;
char *filePath = NULL;
char errorMsg[25];


unsigned short presentCheck;
unsigned short rtList[25], subaddressList[25], wordCountList[25];
unsigned short uniqueRtList[25], uniqueSubList[25], uniqueWCList[25];
unsigned short presentCount = 0;
unsigned short uniqueRtCount = 0, uniqueSubaddrCount = 0, uniqueWordCount = 0;

unsigned short selectedCombo[25][3];
unsigned short selectedCount = 0;

int activate_subaddress_callback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	unsigned short *ind;
	unsigned short value_selected;
	PtGetResource(widget,Pt_ARG_CBOX_SEL_ITEM,&ind,0);
	value_selected = uniqueRtList[*ind - 1];
	PtListDeleteAllItems(ABW_dtana_subaddr_select_combo);
	PtSetResource(ABW_dtana_subaddr_select_combo,Pt_ARG_FLAGS,Pt_FALSE,Pt_BLOCKED|Pt_GHOST);
	uniqueSubaddrCount = 0;

	for(int i = 0; i < presentCount; i++)
	{
		if(value_selected == rtList[i])
		{
			unsigned short subaddrFlag = 0;
			for(int j = 0; j < uniqueSubaddrCount; j++)
			{
				if(subaddressList[i] == uniqueSubList[j])
				{
					subaddrFlag = 1;
					break;
				}
			}
			if(subaddrFlag == 0)
			{
				char a[2];
				uniqueSubList[uniqueSubaddrCount] = subaddressList[i];
				itoa(uniqueSubList[uniqueSubaddrCount],a,10);
				const char* temp[] = {a};
				PtListAddItems(ABW_dtana_subaddr_select_combo,temp,1,0);
				uniqueSubaddrCount++;
			}
		}
	}
	return( Pt_CONTINUE );
}


int activate_wordcount_callback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	unsigned short *ind;
	unsigned short rt_selected;
	unsigned short sub_selected;

	PtGetResource(ABW_dtana_rt_select_combo,Pt_ARG_CBOX_SEL_ITEM,&ind,0);
	rt_selected = uniqueRtList[*ind - 1];
	PtGetResource(widget,Pt_ARG_CBOX_SEL_ITEM,&ind,0);
	sub_selected = uniqueSubList[*ind - 1];

	PtListDeleteAllItems(ABW_dtana_word_count_combo);
	PtSetResource(ABW_dtana_word_count_combo,Pt_ARG_FLAGS,Pt_FALSE,Pt_BLOCKED|Pt_GHOST);
	uniqueWordCount = 0;

	for(int i = 0; i < presentCount; i++)
	{
		if(rt_selected == rtList[i] && sub_selected == subaddressList[i])
		{
			unsigned short wcFlag = 0;
			for(int j = 0; j < uniqueWordCount; j++)
			{
				if(wordCountList[i] == uniqueWCList[j])
				{
					wcFlag = 1;
					break;
				}
			}
			if(wcFlag == 0)
			{
				char a[2];
				uniqueWCList[uniqueWordCount] = wordCountList[i];
				itoa(uniqueWCList[uniqueWordCount],a,10);
				const char* temp[] = {a};
				PtListAddItems(ABW_dtana_word_count_combo,temp,1,0);
				uniqueWordCount++;
			}
		}
	}
	return( Pt_CONTINUE );
}


int realised_callback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	int count;
	char a[2], display_combo[25], temp[5];

	PtGetResource(ABW_dtacq_file_select_text, Pt_ARG_TEXT_STRING, &filePath,0);
	PtSetResource(ABW_dtana_file_selected_display_label, Pt_ARG_TEXT_STRING, filePath,0);

	presentCount = 0;
	uniqueRtCount = 0;
	uniqueSubaddrCount = 0;
	uniqueWordCount = 0;

	fin = fopen(filePath,"r");
	PtListDeleteAllItems(ABW_dtana_rtdisplay_list);
	unsigned short input, read_size;
	read_size = 1;
	while(read_size == 1)
	{
		read_size = fread(&input,sizeof(input),1,fin);
		buffer.usBlkStsWrd = input;
		fread(&input,sizeof(input),1,fin);
		buffer.usTimeTagLSW = input;
		fread(&input,sizeof(input),1,fin);
		buffer.usCmdWrd = input;
		fread(&input,sizeof(input),1,fin);
		count = input - 1;
		cmdWord = *reinterpret_cast<tagCMDWRD*>(&buffer.usCmdWrd);
		if(cmdWord.Tx_Rx == 0)
		{
			fread(buffer.usData,sizeof(input),count,fin);
			fread(&input,sizeof(input),1,fin);
			buffer.usStsWrd1 = input;
		}
		else
		{
			fread(&input,sizeof(input),1,fin);
			buffer.usStsWrd1 = input;
			fread(buffer.usData,sizeof(input),count,fin);
		}
		read_size = fread(&input,sizeof(input),1,fin);
		for(int i = 0; i < presentCount; i++)
		{
			presentCheck = 0;
			if((rtList[i] == cmdWord.RTAddress) && (subaddressList[i] == cmdWord.SubAddr_ModeCode) && (wordCountList[i] == cmdWord.WrdCnt_ModeCode))
			{
				presentCheck = 1;
				break;
			}
		}
		if(presentCheck == 0)
		{
			int flag = 0;
			rtList[presentCount] = cmdWord.RTAddress;
			subaddressList[presentCount] = cmdWord.SubAddr_ModeCode;
			wordCountList[presentCount] = cmdWord.WrdCnt_ModeCode;

			itoa(rtList[presentCount],display_combo,10);
			itoa(subaddressList[presentCount],temp,10);
			strcat(display_combo, "   -    ");
			strcat(display_combo, temp);
			itoa(wordCountList[presentCount],temp,10);
			strcat(display_combo, "   -    ");
			strcat(display_combo, temp);
			const char* temp1[] = {display_combo};
			PtListAddItems(ABW_dtana_rtdisplay_list,temp1,1,0);

			for(int i = 0; i < presentCount; i++)
			{
				if(rtList[presentCount] == rtList[i])
				{
					flag = 1;
					break;
				}
			}
			if (flag == 0)
			{
				uniqueRtList[uniqueRtCount] = rtList[presentCount];
				itoa(uniqueRtList[uniqueRtCount],a,10);
				const char* temp[] = {a};
				PtListAddItems(ABW_dtana_rt_select_combo,temp,1,0);
				uniqueRtCount++;
			}
			PtBkgdHandlerProcess();
			presentCount++;
		}
	}
	fclose(fin);
	return( Pt_CONTINUE );
}


int show_data_callback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	unsigned short msgCount =0;
	unsigned char tr;
	char transType[10];

	fin = fopen(filePath,"r");
	//strcpy(outputFile,"output");
	fout = fopen("Output","w");

	unsigned short input, read_size;
	read_size = 1;
	while(read_size == 1)
	{
		int count;
		read_size = fread(&input,sizeof(input),1,fin);
		buffer.usBlkStsWrd = input;
		fread(&input,sizeof(input),1,fin);
		buffer.usTimeTagLSW = input;
		fread(&input,sizeof(input),1,fin);
		buffer.usCmdWrd = input;
		fread(&input,sizeof(input),1,fin);
		count = input - 1;
		cmdWord = *reinterpret_cast<tagCMDWRD*>(&buffer.usCmdWrd);
		if(cmdWord.Tx_Rx == 0)
		{
			fread(buffer.usData,sizeof(input),count,fin);
			fread(&input,sizeof(input),1,fin);
			buffer.usStsWrd1 = input;
		}
		else
		{
			fread(&input,sizeof(input),1,fin);
			buffer.usStsWrd1 = input;
			fread(buffer.usData,sizeof(input),count,fin);
		}
		read_size = fread(&input,sizeof(input),1,fin);
		msgCount++;
		if(cmdWord.Tx_Rx == 1)
		{
			tr = 'T';
			strcpy(transType,"RT to BC");
		}
		else if(cmdWord.Tx_Rx == 0)
		{
			tr = 'R';
			strcpy(transType,"BC to RT");
		}
		data_check();
		if(selectedCount == 0)
		{
			fprintf(fout,"Msg: %d Type: %s   bsw: %04x    Valid Frame \n",msgCount, transType, buffer.usBlkStsWrd);
			fprintf(fout,"    CMD:%04x    %02d-%c-%02d-%02d  Stat:%04x \n",buffer.usCmdWrd, cmdWord.RTAddress, tr, cmdWord.SubAddr_ModeCode, cmdWord.WrdCnt_ModeCode, buffer.usStsWrd1);
			fprintf(fout,"    ETime:               STime:         Diff_ET:            Diff_ST: \n");
			fprintf(fout,"    Data:   ");
			for(int i = 0; i < cmdWord.WrdCnt_ModeCode; i++)
			{
				fprintf(fout,"%04x ",buffer.usData[i]);
				if(((i + 1) % 8) == 0)
				{
					fprintf(fout,"\n");
					fprintf(fout,"        ");
				}
			}
			fprintf(fout,"\n");
			fprintf(fout,"------------------------------------------");
			fprintf(fout,"\n \n");
		}
		else
		{
			for(int i =0; i < selectedCount; i++)
			{
				if(selectedCombo[i][0] == cmdWord.RTAddress && selectedCombo[i][1] == cmdWord.SubAddr_ModeCode && selectedCombo[i][2] == cmdWord.WrdCnt_ModeCode)
				{
					fprintf(fout,"Msg: %d Type: %s   bsw: %04x    Valid Frame \n",msgCount, transType, buffer.usBlkStsWrd);
					fprintf(fout,"    CMD:%04x    %02d-%c-%02d-%02d  Stat:%04x \n",buffer.usCmdWrd, cmdWord.RTAddress, tr, cmdWord.SubAddr_ModeCode, cmdWord.WrdCnt_ModeCode, buffer.usStsWrd1);
					fprintf(fout,"    ETime:               STime:         Diff_ET:            Diff_ST: \n");
					fprintf(fout,"    Data:   ");
					for(int i = 0; i < cmdWord.WrdCnt_ModeCode; i++)
					{
						fprintf(fout,"%04x ",buffer.usData[i]);
						if(((i + 1) % 8) == 0)
						{
							fprintf(fout,"\n");
							fprintf(fout,"        ");
						}
					}
					fprintf(fout,"\n");
					fprintf(fout,"------------------------------------------");
					fprintf(fout,"\n \n");
				}
			}
		}
	}
	fclose(fin);
	fclose(fout);
	spawnlp(P_NOWAIT,"ped","ped","Output",NULL);
	return( Pt_CONTINUE );
}


int add_combo_callback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	char display_combo[25], temp[5];
	unsigned short selected[25][3];
	char Helvetica12[MAX_FONT_TAG];
	unsigned short *ind1, *ind2, *ind3, inAdded = 0;

	PtGetResource(ABW_dtana_rt_select_combo,Pt_ARG_CBOX_SEL_ITEM,&ind1,0);
	PtGetResource(ABW_dtana_subaddr_select_combo,Pt_ARG_CBOX_SEL_ITEM,&ind2,0);
	PtGetResource(ABW_dtana_word_count_combo,Pt_ARG_CBOX_SEL_ITEM,&ind3,0);

	if(*ind1 != 0 && *ind2 != 0 && *ind3 != 0)
	{
		selected[inAdded][0] = uniqueRtList[*ind1 -1];
		selected[inAdded][1] = uniqueSubList[*ind2 -1];
		selected[inAdded][2] = uniqueWCList[*ind3 -1];
		inAdded++;
	}
	else if(*ind1 != 0 && *ind2 != 0 && *ind3 == 0)
	{
		unsigned short rt = uniqueRtList[*ind1 -1];
		unsigned short sub = uniqueSubList[*ind2 -1];
		for(int i = 0; i < presentCount; i++)
		{
			if(rt == rtList[i] && sub == subaddressList[i])
			{
				selected[inAdded][0] = rtList[i];
                selected[inAdded][1] = subaddressList[i];
                selected[inAdded][2] = wordCountList[i];
                inAdded++;
			}
		}
		PtNotice( ABW_data_analysis_window, NULL, "Information", NULL,"All word counts for the this RT Sub address combination, selected ",PfGenerateFontName("Helvetica", 0, 12, Helvetica12),NULL, NULL, Pt_BLOCK_PARENT|Pt_WAIT|Pt_RELATIVE|Pt_ESC_DISABLE );
	}
	else if(*ind1 != 0 && *ind2 == 0)
	{
		unsigned short rt = uniqueRtList[*ind1 -1];
		for(int i = 0; i < presentCount; i++)
		{
			if(rt == rtList[i])
			{
				selected[inAdded][0] = rtList[i];
				selected[inAdded][1] = subaddressList[i];
				selected[inAdded][2] = wordCountList[i];
				inAdded++;
			}
		}
		PtNotice( ABW_data_analysis_window, NULL, "Information", NULL,"All sub address and word count combinations for this RT, selected",PfGenerateFontName("Helvetica", 0, 12, Helvetica12),NULL, NULL, Pt_BLOCK_PARENT|Pt_WAIT|Pt_RELATIVE|Pt_ESC_DISABLE );
	}
	else if(*ind1 == 0)
	{
		for(int i = 0; i < presentCount; i++)
		{
			selected[inAdded][0] = rtList[i];
			selected[inAdded][1] = subaddressList[i];
			selected[inAdded][2] = wordCountList[i];
        	inAdded++;
 		}
		PtNotice( ABW_data_analysis_window, NULL, "Information", NULL,"All combinations selected",PfGenerateFontName("Helvetica", 0, 12, Helvetica12),NULL, NULL, Pt_BLOCK_PARENT|Pt_WAIT|Pt_RELATIVE|Pt_ESC_DISABLE );
	}

	for(int j = 0; j < inAdded; j++)
	{
		int flag = 0;
		for(int i = 0; i < selectedCount; i++)
		{
			if(selected[j][0] == selectedCombo[i][0] && selected[j][1] == selectedCombo[i][1] && selected[j][2] == selectedCombo[i][2])
			{
				flag = 1;
				break;
			}
		}
		if(flag == 0)
		{
			selectedCombo[selectedCount][0] = selected[j][0];
			itoa(selectedCombo[selectedCount][0],display_combo,10);
			selectedCombo[selectedCount][1] = selected[j][1];
			itoa(selectedCombo[selectedCount][1],temp,10);
			strcat(display_combo, "   -    ");
			strcat(display_combo, temp);
			selectedCombo[selectedCount][2] = selected[j][2];
			itoa(selectedCombo[selectedCount][2],temp,10);
			strcat(display_combo, "   -    ");
			strcat(display_combo, temp);

			selectedCount++;
			PtListDeleteAllItems(ABW_dtana_subaddr_select_combo);
			PtListDeleteAllItems(ABW_dtana_word_count_combo);
			PtSetResource(ABW_dtana_subaddr_select_combo,Pt_ARG_FLAGS,Pt_TRUE,Pt_BLOCKED|Pt_GHOST);
			PtSetResource(ABW_dtana_word_count_combo,Pt_ARG_FLAGS,Pt_TRUE,Pt_BLOCKED|Pt_GHOST);

			const char* temp1[] = {display_combo};
			PtListAddItems(ABW_dtana_selected_combo_list,temp1,1,0);
		}
	}
	return( Pt_CONTINUE );
}

/*
 *
 *
 * User defined functions for RT manipulation
 *
 *
 */
int data_check()
{
	blkStsWrd blk;
	blk =  *reinterpret_cast<blkStsWrd*>(&buffer.usBlkStsWrd);
	if(buffer.usBlkStsWrd == 0x8100 || buffer.usBlkStsWrd == 0x8180)
		strcpy(errorMsg,"Valid Frame");
	if(blk.cmdWrdContentErr == 1)
		strcpy(errorMsg,"Command Word Err");
	if(blk.rtRtSecondCmdErr == 1)
		strcpy(errorMsg,"Rt to Rt Command Err");
	if(blk.rtRtAddrErr == 1)
		strcpy(errorMsg,"GAP/SYNC/ADDR Err");
	if(blk.invWrd == 1)
		strcpy(errorMsg,"Invalid Word");
	if(blk.wrdCntErr == 1)
		strcpy(errorMsg,"Word Count Err");
	if(blk.reserved == 1)
		strcpy(errorMsg,"Reserved bit Filled");
	if(blk.goodDataBlkTransfer == 0)
		strcpy(errorMsg,"Bad Data");
	if(blk.errFlag == 1)
	{
		if(blk.noResTimeout == 1)
		{
			strcpy(errorMsg,"Response Timeout");
		}

		else if(blk.formatErr == 1)
		{
			strcpy(errorMsg,"Format Err");
		}

		else
		{
			printf("Handshake Failure");
		}
	}

	return 0;
}

int dtana_cancel_callback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	PtDestroyWidget(ABW_data_acquisition_window);
	return( Pt_CONTINUE );
}

int cancel_callback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	PtDestroyWidget(ABW_data_analysis_window);
	return( Pt_CONTINUE );
}


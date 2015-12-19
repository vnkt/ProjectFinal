/*
 * common_utils.cpp
 *
 *  Created on: Mar 12, 2011
 *      Author: Aneesh.K.Thampi
 *      NCSAD/LVISG/IISU/ISRO
 *
 *      Description: Common utilities for photon based applications
 *      Update ver1.0  :27/04/11
 *      Update Ver1.1  ;04/05/11 bug related to col pos in Panel_Display corrected in Update_Index
 *      Update ver 1.2 ;29/11/11 added variable argument version for Panel_Dispaly, wprintf,wgprintf
 */
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "common_utils.hpp"
#include "my_widget.hpp"
//==============================================
//Panel_Display Class methods
// To display messages in rows and columns
//===============================================
 Panel_Display :: Panel_Display()
{
	//int argc =0;
	//PtArg_t  args[5];
	ptr = NULL;
}

 void Panel_Display :: Init(PtWidget_t *wid)
 {
	 int argc =0;
	 PtArg_t  args[5];
	 if (ptr != NULL)
		 return;
	 ptr = wid;

	 //Disable char and word wrapping
     PtSetArg(&args[argc++], Pt_ARG_MULTITEXT_WRAP_FLAGS,Pt_FALSE, Pt_EMT_CHAR |Pt_EMT_WORD);
     //Enable new line wrapping
     PtSetArg(&args[argc++], Pt_ARG_MULTITEXT_WRAP_FLAGS,Pt_TRUE, Pt_EMT_NEWLINE);
     //left align
     PtSetArg(&args[argc++], Pt_ARG_HORIZONTAL_ALIGNMENT,Pt_LEFT, 0);
     //to disable selection scroll
     PtSetArg(&args[argc++], Pt_ARG_MULTITEXT_FLAGS,Pt_FALSE, Pt_EMT_SCROLL_TO_CURSOR);

    //------
    //masked only for debug
     PtSetArg (&args[argc++], Pt_ARG_TEXT_FLAGS,Pt_FALSE,Pt_TEXT_BLINKING_CURSOR|Pt_EDITABLE|Pt_CURSOR_VISIBLE);
     PtSetResources( ptr, argc, args );

     Clear();


  }
//to find out the start and end point new text in the text buffer
flag_t Panel_Display :: Update_Index (const char *str,char in_row,char in_col)
 {

	 unsigned short r_temp;

	 if ( in_row > MAX_ROWS || in_col > MAX_COLUMN || in_row < 0 || in_col<0)
	 {
		 printf ("Invalid row or column\n");
		 return  1;
	 }

	 start = (in_row -1)*(MAX_COLUMN+1) + in_col -1 ;
	 end = start + strlen(str)- 1;
	 length = strlen(str);

	 //check start and end corresponds to same line
	 //there are MAX_COLUMN +1 chars per line(including default new line at the end)
	 r_temp = start /(MAX_COLUMN+1);    //+1 added to fix bug
	 if (r_temp != end/(MAX_COLUMN+1))  //+1 added in ver1.1
	 {
		 r_temp = start % (MAX_COLUMN +1);
		 length = (MAX_COLUMN - r_temp);
		 end = start + length -1;
	 }

/*	 if ( start == end)  //special case write a char
	 	 {
	 		 Modify_Single_Char(str[0]);  //at the location 'start'
	 		 return 1;                       // no need to update in Put();
	 	 }
*/
	 curr_row ++;
	 return 0;

 }

void   Panel_Display::Set_Font_Attr(PgColor_t color,PgColor_t back_color,char font_attr)
{
	unsigned short index =0;
	char b=0,i=0,bold=0,italic=0;
	mask = 0;
	if (font_attr & DISPLAY_BOLD )
		bold =1;

	if(font_attr & DISPLAY_ITALIC)
		italic=1;

	//if no valid args use default font settings
	//if(!(color|back_color|bold|italic))
//	{
		attr.font = Pt_DEFAULT_FONT;
		attr.background_color = Pt_DEFAULT_COLOR;
		attr.text_color = Pt_DEFAULT_COLOR;
		mask = Pt_MT_TEXT_COLOR |Pt_MT_BACKGROUND_COLOR|Pt_MT_FONT;
	//	return ;
	//}

	if (color != 0)
		{
		//mask = mask |Pt_MT_TEXT_COLOR;
		attr.text_color       = color;
		}

	if( back_color != 0)
		{
		//mask = mask|Pt_MT_BACKGROUND_COLOR;
		attr.background_color = back_color;
		}


	if( bold || italic)
	{
		Get_Font(ptr,font);
			index= strlen(font);
		//find the current status
		if(strchr(font+index-2,'b') != NULL)
			if(font[index-1] == 'b')
			{
				b=1;
				i=0;
			}
			else
			{
				b=1;
				i=1;
			}
		else
		{
			if (font[index-1] == 'i')
				i=1;
		}

		//set the new attr

		if (bold && !b)
		{
			if(i)
			{
				font[index-1]='b';
				strcat(font,"i");
			}
			else
				strcat(font,"b");
		}
		if(italic && !i)
			strcat(font,"i");

		attr.font =font;
		mask = mask|Pt_MT_FONT;
	}
	/*
	else
	attr.font = Pt_DEFAULT_FONT;
	attr.background_color = back_color;
	attr.text_color       = color;
	*/
	return;

}






void Panel_Display::Modify_Single_Char(char c)  //at location 'start'
{
	Get_Text(ptr,dis_buf);
	dis_buf[start] = c;
	Set_Color_Text(ptr,0,dis_buf);

}

void Panel_Display :: Dis (const char *str,char in_row,char in_col,PgColor_t color,PgColor_t back_color,char font_attr)
{

	char buf[MAX_COLUMN],*pt=NULL;
	if ( in_row > MAX_ROWS || in_col > MAX_COLUMN || in_row < 0 || in_col<0)
			 {
				 printf ("Invalid row or column\n");
			 	 return  ;
			 }
		 if (in_row == 0)
			 in_row = curr_row;
		 else
			 curr_row = in_row;
		 if (in_col == 0)
			 in_col =1;

	if (strlen(str) == 0)
		return;

   Set_Font_Attr(color,back_color,font_attr);

	if ( strchr(str,'\n') == NULL)  //no new line chars
	{
		if(!Update_Index(str,in_row,in_col))
			PtMultiTextModifyText (ptr,start,end+1,start,str,length,&attr,mask); //end +1 is required because it deletes chars from
	                                                                    //   start to end (but not including end)
	}
	else
	{
		//split the string into tokens
		strcpy (buf,str);
		if(buf[0] == '\n')
			in_row++;
		pt = strtok(buf,"\n");
		do
		{
			if(!Update_Index(pt,in_row++,in_col))
				PtMultiTextModifyText (ptr,start,end+1,start,pt,length,&attr,mask);  //end +1 is required because it deletes chars from
			                                                                         //   start to end (but not including end)
             in_col =1;
             pt = strtok(NULL,"\n");
		}while(pt != NULL);

	}

}

void Panel_Display::wgprintf(char row,char col,PgColor_t color,PgColor_t back_color,char font_attr,const char *fmt,...)
{
	char str[101];
	va_list arglist;
	va_start(arglist,fmt);
	vsnprintf(str,100,fmt,arglist);
	Dis(str,row,col,color,back_color,font_attr);
	va_end(arglist);
}

void Panel_Display::wprintf(char row,char col,const char *fmt,...)
{
	char str[101];
	va_list arglist;
	va_start(arglist,fmt);
	vsnprintf(str,100,fmt,arglist);
	Dis(str,row,col);
	va_end(arglist);
}

void Panel_Display::Clear(char in_row,char in_col)
{
	short start,len =0;
	unsigned short i=0,j=0;
	if(in_row > 0)
		in_row--;
	else
		in_row=0;

	if(in_col > 0)
			in_col--;
		else
			in_col=0;

	start = (in_row * (MAX_COLUMN+1)) + in_col;
	len   = MAX_ROWS*(MAX_COLUMN +1) - start;


//	memset(dis_buf,' ',(MAX_COLUMN+1) * MAX_ROWS);

	memset(dis_buf+ start,' ',len);
	for (i=0,j=0;i<MAX_ROWS;i++,j=j+MAX_COLUMN+1)
	{
    	dis_buf[j+MAX_COLUMN] = '\n';
	}
	dis_buf[j] = '\0';


	len = strlen(dis_buf+start);

	PtTextModifyText( ptr,start,start+len-1,-1,dis_buf,len );
//	curr_row =1;  //first row org
	curr_row= in_row+1;

}
//End of Panel_Display Class methods
//===================================

//To get or save value to a file
//Eg: To save the index of the user selected widget
//val has default argument INVALID ,which denotes read operation
short Get_Sav_Val_File(const char *fname,short val)
{
	FILE *fp;
	if(val!=INVALID)   //save mode
	fp = fopen(fname,"wb");
	else
	fp = fopen(fname,"rb");
	if (fp==NULL && val!=INVALID)  //sav the value
	{
		printf ("File open Error:%s\n",fname);
		return ERROR;
	}
	if (fp == NULL) //read mode
		return INVALID;

	if(val!=INVALID)
		fwrite(&val,sizeof(short),1,fp);
	else  //read one value and return
		{
		fread(&val,sizeof(short),1,fp);
		fclose(fp);
		return val;
		}
	fclose(fp);
	return NO_ERR;

}

//To get the index of the given widget pointer in a given array
//Eg:To identify the widget in the callback

flag_t Get_Widget_Index(PtWidget_t *wid,int *array,flag_t num)
{
	int abn_num;
	short i;
	abn_num = ApName(wid);

	for (i=0;i<num;i++)
	{
		if(array[i]==abn_num)
			return i;
	}
	return INVALID;
}




//to extract the string between the delimiter character
//offset is the distance (in bytes) to the second occurrence of the delimiter character
flag_t Get_String_Token(const char *in_string,char *out_string,char delim,flag_t *end_offset)
{
	const char *strt,*end;
    byte len;
	//const char *test_string ="  \"ok\"hk";
	if ((strt=strchr(in_string,delim)) == NULL)
		return ERROR;
	if ((end=strchr(strt+1,delim)) == NULL)
			return ERROR;
	len = end -strt-1;
	*end_offset =(char) (end-in_string) +1;
	strncpy(out_string,strt+1,len);
	out_string[(int)len] = '\0';
	return NO_ERR;

}

flag_t Block_Window(PtWidget_t *wnd,flag_t block)
{
	static PtBlockedList_t *blklist=NULL;
	char ret=NO_ERR;
	if(block) //to block
		{
		blklist = PtBlockWindow(wnd,Ph_CURSOR_NOINPUT,0 );
		if (!blklist)
			ret = ERROR;
		}
	else //unblock
	{
		if(blklist)
		   {
			PtUnblockWindows(blklist);
			blklist = NULL;
		   }
		else
			ret=ERROR;
	}
	return ret;
}



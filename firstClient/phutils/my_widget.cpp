/*
 * my_widget.cpp
 *
 *  Created on: Mar 12 2011
 *      Author: Aneesh. K .thampi
 *      Update:06-07-11  MyDialog->Get_text()
 *             len+1 is passed to Get_Input()                   tested:ok
 *      Update:07-07-11 MyDialog->msgbox()
 *             bt_font replaced with NULL on call to PtAlert(), due to serious display error
 *             It needs to be corrected.
 *             07-07-11 MyDialog->msgbox()
 *              if parent is null, flags passed to PtAlert will result in segmentation falt: Fixed
 *      Update 09-07-11
 *             GetUserData bug fixed
 *             Combo load from file: bug fixed
 *
 */


#include "my_widget.hpp"
#include <stdarg.h>
#include <stdio.h>


//miscellaneous
 short int Get_Text(PtWidget_t *wid,char *ret_text )
{

	const char *str;
	if(PtGetResource( wid, Pt_ARG_TEXT_STRING, &str, 0 ) == 0)
	{
		if (ret_text != NULL)
		strcpy(ret_text,str);
		return strlen(str);
	}
	return INVALID;
}
 //...................................................
 void Set_Color_Text(PtWidget_t *wid,PgColor_t Color,const char *fmt,...)
 {
	 if(!wid)
		 return;
	 char str[61];
	 va_list arglist;
	 va_start(arglist,fmt);
	 vsnprintf(str,60,fmt,arglist);
	 PtSetResource( wid, Pt_ARG_TEXT_STRING, str, 0 );
	 if (Color)
		 PtSetResource(wid,Pt_ARG_COLOR,Color,0);
	 va_end(arglist);
 }
 //.......................................................

 void    Set_Window_Title(PtWidget_t *wid,const char *str)
 {
	PtSetResource( wid, Pt_ARG_WINDOW_TITLE, str, 0 );
 }

 short int Get_Text_Ptr(PtWidget_t *wid,char *buf )
 {
	 PtGetResource( wid, Pt_ARG_TEXT_STRING, &buf, 0 );
	 return strlen(buf);
 }
 //______________________________________________________

 void Set_Numeric_Int (PtWidget_t *wid,long int val)
 {
	 PtSetResource( wid, Pt_ARG_NUMERIC_VALUE, val, 0 );

 }

 //________________________________________________________

flag_t Get_Numeric_Int (PtWidget_t *wid,long int * val)
{
	long int *num;
	if(PtGetResource (wid, Pt_ARG_NUMERIC_VALUE,&num,0) == 0)
	{
		*val = *num;
		return NO_ERR;
	}
	else
		return ERROR;
}

//___________________________________________________________
flag_t Get_Numeric_Int (PtWidget_t *wid,int * val)
{
	long int val1;
	if (Get_Numeric_Int(wid,&val1))
		return ERROR;
	*val = (int)val1;
	return NO_ERR;

}

//_____________________________________________________________
void Set_Numeric_Float(PtWidget_t *wid,double val)
{
	PtSetResource( wid, Pt_ARG_NUMERIC_VALUE, val, 0 );

}

//To get combo box list index
flag_t Get_List_Index(PtWidget_t *wid)
{
	unsigned short *ind;
	PtGetResource(wid,Pt_ARG_CBOX_SEL_ITEM,&ind,0);
	return (flag_t)*ind;
}

void Set_Widget_State (PtWidget_t *wid,flag_t flg)
{
	if(flg)
	  PtSetResource(wid,Pt_ARG_FLAGS,Pt_TRUE,Pt_SET);
	else
	  PtSetResource(wid,Pt_ARG_FLAGS,Pt_FALSE,Pt_SET);

}

//____________________________________________________________
/*
void Set_Numeric_Float(PtWidget_t *wid,float val)
{
	PtSetResource(wid,Pt_ARG_NUMERIC_VALUE,(double)val,0);
}
*/

flag_t Get_Numeric_Float(PtWidget_t *wid,float *val)
{
	double *number;

	if(PtGetResource (wid, Pt_ARG_NUMERIC_VALUE,&number, 0))
		return ERROR;
	*val = (float)(*number);
	return NO_ERR;

}
//_______________________________________________________________
flag_t Get_Numeric_Float(PtWidget_t *wid,double *val)
{
	if(PtGetResource (wid, Pt_ARG_NUMERIC_VALUE,&val, 0))
			return ERROR;
	return NO_ERR;

}
//________________________________________________________________

char  Get_Font (PtWidget_t *wid,char *ret_font)
{
	const char *str;
		if(PtGetResource( wid, Pt_ARG_TEXT_FONT, &str, 0 ) == 0)
		{
			strcpy(ret_font,str);
			return (char)strlen(str);
		}
		return -1;

}

//____________________________________________________________________
flag_t Get_User_Data(PtWidget_t *wid,char *data)
{
	const char *str=NULL;
	strcpy(data,"");
	PtGetResource( wid, Pt_ARG_USER_DATA, &str, 0 );
	if(str != NULL)
	strcpy(data,str);
	return strlen(data);
}

flag_t Check_Widget_Selection(PtWidget_t *wid)
{
	long flg;
	flg = PtWidgetFlags(wid);
	if(flg & Pt_SET)
		return 1;
	return 0;
}

void    Enable_Disable_Widget(PtWidget_t *wid,flag_t flg)
{
	if(flg)
		PtSetResource(wid,Pt_ARG_FLAGS,Pt_FALSE,Pt_BLOCKED|Pt_GHOST);  //enable
	else
		PtSetResource(wid,Pt_ARG_FLAGS,Pt_TRUE,Pt_BLOCKED|Pt_GHOST);   //disable
	return;
}

//_________________________________________________
//To hide a widget, set its position co-ordinates to a large negative number
//and block it from getting the focus
void Hide_Widget(PtWidget_t *wid)
{
	PhPoint_t point;
	point.x=-1000;
	point.y=-1000;
	PtSetResource(wid,Pt_ARG_POS,&point,0);
	PtSetResource(wid,Pt_ARG_FLAGS,Pt_BLOCKED,Pt_BLOCKED);
	//PtSetRes
}

//To hide window
void Hide_Window(PtWidget_t *wid)
{
	PtSetResource(wid,Pt_ARG_WINDOW_STATE,Pt_TRUE,Ph_WM_STATE_ISHIDDEN);
}
void Show_Window(PtWidget_t *wid)
{
	PtSetResource(wid,Pt_ARG_WINDOW_STATE,Pt_TRUE,Ph_WM_STATE_ISNORMAL);

}
//CallBack related
//To add one callback to many widgets
//list contains the widget pointers and tags
//count is the number of widgets
void Add_Callback(Callback_List_t *list,short count,int (*callback)(PtWidget_t *widget,void *apinfo,PtCallbackInfo_t *cbinfo))
{
	int i;
	for (i=0 ;i<count;i++)
	 PtAddCallback((list+i)->wid_ptr,Pt_CB_ARM,callback,(void*)(list+i)->tag);

}

//Display values in Widgets with limit check
//__________________________________________________

 My_Widget::My_Widget(PtWidget_t *wid)
{
	wid_ptr = wid;
	limit_status =0;
	Err_Color = Pg_RED;
}

 My_Widget::My_Widget(PtWidget_t *wid,const int min,const int max,PgColor_t Color)
 {
	 wid_ptr = wid;
	 limit.int_limit.max = max;
	 limit.int_limit.min = min;
	 limit_status = INTEGER_VALUE;
	 Err_Color=Color;                   //default red
	 PtGetResource(wid,Pt_ARG_COLOR,&Color_Ptr,0);  //save the original color
	 Org_Color = *Color_Ptr;
 }

 My_Widget::My_Widget(PtWidget_t *wid,const double min,const double max,PgColor_t Color)
 {
	 wid_ptr = wid;
    limit_status = FLOAT_VALUE;
	limit.float_limit.max = max;
	limit.float_limit.min = min;
	Err_Color = Color;
	PtGetResource(wid,Pt_ARG_COLOR,&Color_Ptr,0);
	Org_Color = *Color_Ptr;
 }

short My_Widget::Get_Data(char *text)
 {
	 if (limit_status)  //int or float widget
		 return INVALID;
	 return Get_Text(wid_ptr,text);
 }

flag_t My_Widget ::Get_Data(long int *val)
{
	return Get_Numeric_Int(wid_ptr,val);
}

flag_t My_Widget::Get_Data(int *val)
{
	return Get_Numeric_Int(wid_ptr,val);
}

flag_t My_Widget ::Get_Data(float *val)
{
	return Get_Numeric_Float(wid_ptr,val);
}

flag_t My_Widget ::Get_Data(double *val)
{
	return Get_Numeric_Float(wid_ptr,val);
}

void My_Widget::Set_Limit_Hit(flag_t status)
{
	if(status)
	 {
	   PtSetResource(wid_ptr,Pt_ARG_COLOR,Err_Color,0);
	   limit_status |= LIMIT_EXCEEDED;
	 }
	 else
	 {
		 if(limit_status & LIMIT_EXCEEDED)   //reset the error color
		 {
			 PtSetResource(wid_ptr,Pt_ARG_COLOR,Org_Color,0);
			 limit_status &= ~LIMIT_EXCEEDED;
		 }
	 }
 }


 void My_Widget :: Set_Data(const int val)
 {
	 if (limit_status & INTEGER_VALUE)
	    if (val < limit.int_limit.min || val > limit.int_limit.max)
	 	 Set_Limit_Hit(TRUE);
	    else
	    Set_Limit_Hit(FALSE);
	 Set_Numeric_Int(wid_ptr,val);

 }
 void My_Widget :: Set_Data(const double val)
  {
 	 if (limit_status & FLOAT_VALUE)
 	    if (val < limit.float_limit.min || val > limit.float_limit.max)
 	 	   Set_Limit_Hit(TRUE);
 	    else
 	    	Set_Limit_Hit(FALSE);

 	 Set_Numeric_Float(wid_ptr,val);

  }


 // Following are applicable for labels and text boxes
 void My_Widget::Set_Text(const char *fmt,...)
 {
	 va_list arglist;

	 va_start(arglist,fmt);
	 vsnprintf(str,50,fmt,arglist);
	 PtSetResource( wid_ptr, Pt_ARG_TEXT_STRING, str, 0 );
	 va_end(arglist);

 }
 //set text with limit check
 void  My_Widget::Set_Text_chk(const char *fmt,const double val)
 {
	 snprintf(str,50,fmt,val);

	 if (limit_status & FLOAT_VALUE)
		if (val < limit.float_limit.min || val > limit.float_limit.max)
			Set_Limit_Hit(TRUE);
		else
			Set_Limit_Hit(FALSE);

	 PtSetResource( wid_ptr, Pt_ARG_TEXT_STRING, str, 0 );

 }

 void  My_Widget::Set_Text_chk(const char *fmt,const int val)
 {
	 snprintf(str,50,fmt,val);
	 if (limit_status & INTEGER_VALUE)
	   if (val < limit.int_limit.min || val > limit.int_limit.max)
		  Set_Limit_Hit(TRUE);
	   else
		  Set_Limit_Hit(FALSE);

	 PtSetResource( wid_ptr, Pt_ARG_TEXT_STRING, str, 0 );

 }


 //=================================================================
 //Commonly used dialogs
 //


/*
 My_Dialog::My_Dialog(const char *db_filename)
 {
	 parent = NULL;
	 image_ptr=NULL;
	 font=NULL;
	 dbase = ApOpenDBaseFile(db_filename);
	 ApAddClass( "PtLabel",&PtLabel);  //to add possible widget in the dbfile, to internal widget class table
	 	                                  //PTlabel containing image is used here
 }
 */





 flag_t My_Dialog::Init(ApEventLink_t const *link_instance)
 {
	 parent=NULL;font=NULL;

	 Set_Font("TextFont",PF_STYLE_BOLD,10);
	 dbase = ApOpenDBase(link_instance);
	 if(dbase)
		 return NO_ERR;
	 return ERROR;

 }
 flag_t My_Dialog ::Get_Picture(flag_t type)
 {
	 char pict_name[20];
	 image_ptr = NULL;
	 if(dbase)
	 {
		 sprintf(pict_name,"picture%d",type);
		 image_ptr = ApGetImageRes( dbase, pict_name);
	// image_ptr = ApGetImageRes( dbase, stored_images[type] );
	 }
	 if(image_ptr)
		 return NO_ERR;
	 return ERROR;

 }
flag_t My_Dialog::  Set_Font(char const *pkge,unsigned int flg,unsigned int size)
 {
	 if(PfGenerateFontName(pkge, flg, size,font_text)!= NULL)
	    font = font_text;
	 else
		 return ERROR;
	 return NO_ERR;
 }


flag_t My_Dialog::Get_Input(const char *title,const char* msg,char *data,int cnt)
{
	char answer;
	const char *buttons[] = {"&Cancel","&Ok"};
	answer = PtPrompt(parent,NULL,title,image_ptr,msg,font,2,buttons,NULL,2,1,cnt,data,NULL,NULL,Pt_RIGHT|Pt_BLOCK_ALL|Pt_RELATIVE);
	if (answer == 2)
		return NO_ERR;
	return ERROR;
}

flag_t My_Dialog::Get_Int(int &val,const char *msg)
{
	sprintf(buf,"%d",val);
	Get_Picture(PH_PICT_PROJECT);

	if (Get_Input("Input Integer",msg,buf,10))
		return ERROR;
	if(sscanf(buf,"%d",&val)!=1)
	{
		val =0;
		return ERROR;
	}
	return NO_ERR;
}


flag_t My_Dialog::Get_Float(float &val,const char *msg,const char *fmt)
{
	sprintf (buf,fmt,val);
//	PfGenerateFontName("Helvetica", 0, 9,font);
	Get_Picture(PH_PICT_PROJECT);  //set picture

	if (Get_Input("Input Integer",msg,buf,10))
		return ERROR;
	if(sscanf(buf,"%f",&val) !=1)
	{
	 val =0.0;
	 return ERROR;
	}
	return NO_ERR;
}
flag_t My_Dialog::Get_Text(char *val,const char *msg,short len)
{
	//PfGenerateFontName("Helvetica", 0, 9,font);
	Get_Picture(PH_PICT_PROJECT); //set picture
	if (Get_Input("Input Text",msg,val,len+1))
			return ERROR;
	//strcpy(val,buf);
	return NO_ERR;
}

flag_t My_Dialog::msgbox(const char *msg,const char *title,flag_t type,flag_t def_button,flag_t pict)
{
	short response;
	int flags=0;
	const char *ph_ok[]={"&OK"},*ph_ok_cancel[]={"&OK","&Cancel"},
	                     *ph_yes_no[]={"&Yes","&No"},
	                     *ph_yes_no_cancel[]={"&Yes","&No","&Cancel"};
	const char **buttons[] = {ph_ok,ph_ok_cancel,ph_yes_no,ph_yes_no_cancel};
	short int button_cnt[] = {1,2,2,3};

	//check the input validity
	if (type > 3)  //maximum supported now
		{
		type =1;
		def_button =1;
		}
	if (def_button > button_cnt[type])
		def_button = button_cnt[type];
	Get_Picture(pict);  //set picture

	//const char *bt_font[]={font,font,font}; //removed due to bug
	//following statement a special case because PH_CANCEL is defined as 3 to avoid conflict with PH_NO
	if ((type == PH_OK_CANCEL) && (def_button == PH_CANCEL))
		def_button = 2;

	if(parent != NULL)
	flags =Pt_CENTER|Pt_RELATIVE|Pt_BLOCK_PARENT;//|Pt_BLOCK_ALL
	/*if parent is not NULL, these flags will harm .bug fix on 7-07-11*/

	response = PtAlert (parent,NULL,title,image_ptr,
			            msg,NULL,button_cnt[type],buttons[type],NULL,
			            def_button,def_button,flags);

	//a special case because PH_CANCEL is defined as 3 to avoid conflict with PH_NO
	if ((type == PH_OK_CANCEL) && (response == 2))
		return PH_CANCEL;


	return (flag_t)response;
}
//variable argument version of msgbox
flag_t My_Dialog::msgbox(const char *title,flag_t type,flag_t def_button,flag_t pict,const char *msg,...)
{
	flag_t ret;
	char str[101];
	va_list arglist;
	va_start(arglist,msg);
	vsnprintf(str,100,msg,arglist);
	ret = msgbox(str,title,type,def_button,pict);
	va_end(arglist);
	return ret;
}
//msgbox with user defined buttons
flag_t My_Dialog::msgbox(const char *msg,const char *title,flag_t pict,const char *buttons[],short int btcount,flag_t def_button)
{
	int flags=0;
	short response;
	Get_Picture(pict);  //set picture
	if(parent != NULL)
		flags =Pt_CENTER|Pt_RELATIVE|Pt_BLOCK_PARENT;//|Pt_BLOCK_ALL
		/*if parent is not NULL, these flags will harm .bug fix on 7-07-11*/
	response = PtAlert (parent,NULL,title,image_ptr,
				            msg,NULL,btcount,buttons,NULL,
				            def_button,def_button,flags);
	return (flag_t)response;

}
/*
flag_t My_Dialog::dis_msg(const char *msg,const char *title,flag_t type,const char *btntext)
{

	Get_Picture(type);
	//PfGenerateFontName("Helvetica", 0, 9,font);
	PtNotice(parent,NULL,title,image_ptr,msg,font,btntext,font,Pt_CENTER|Pt_BLOCK_ALL);//Pt_LEFT|Pt_BLOCK_ALL|Pt_RELATIVE);
	return NO_ERR;
}
*/
void My_Dialog::Dis_Msg(const char *title,flag_t pict,const char *fmt,...)
{
	 char str[61];
	 va_list arglist;
	 va_start(arglist,fmt);
	 vsnprintf(str,60,fmt,arglist);
	 Get_Picture(pict);
	 if(parent != NULL)
	 PtNotice(parent,NULL,title,image_ptr,str,NULL,"&Ok",NULL,Pt_CENTER|Pt_BLOCK_PARENT|Pt_RELATIVE);
	 else
	 PtNotice(parent,NULL,title,image_ptr,str,NULL,"&Ok",NULL,0);
	 va_end(arglist);

}
void My_Dialog::Dis_Msg_Wait(const char *title,flag_t pict,const char *fmt,...)
{
	 char str[61];
	 va_list arglist;
	 va_start(arglist,fmt);
	 vsnprintf(str,60,fmt,arglist);
	 Get_Picture(pict);
	 if(parent != NULL)
	 PtNotice(parent,NULL,title,image_ptr,str,NULL,"&Ok",NULL,Pt_CENTER|Pt_BLOCK_PARENT|Pt_WAIT|Pt_RELATIVE);
	 else
	 PtNotice(parent,NULL,title,image_ptr,str,NULL,"&Ok",NULL,Pt_WAIT);
	 va_end(arglist);

}


//Get the password and validate
int validate( void *data,char const *password_entered )
{
	if (strcmp(password_entered,(const char *)data) == 0)
		return Pt_PWD_ACCEPT;

	return Pt_PWD_RETRY;

}

flag_t My_Dialog:: Chk_Password(const char *exp_password,const char *fmt...)
{
	char str[61];
	int flags =0;
	va_list arglist;
	va_start(arglist,fmt);
	vsnprintf(str,60,fmt,arglist);
	Get_Picture(PH_PICT_PASSWORD);
	if(parent != NULL)
		flags =Pt_CENTER|Pt_RELATIVE|Pt_BLOCK_PARENT;//|Pt_BLOCK_ALL
	if(PtPassword(parent,NULL,"Password Protected",image_ptr,str,font,NULL,NULL,NULL,validate,(void *)exp_password,NULL,flags) == Pt_PWD_ACCEPT)
	 return NO_ERR;
	else
	 return ERROR;
}



void Dis_Msg(PtWidget_t *parent,const char *title,const char *fmt,...)
{
	 char str[61];
	 va_list arglist;
	 va_start(arglist,fmt);
	 vsnprintf(str,60,fmt,arglist);
	 if(parent != NULL)
	 PtNotice(parent,NULL,title,NULL,str,NULL,NULL,NULL,Pt_CENTER|Pt_BLOCK_ALL);
	 else
	 PtNotice(parent,NULL,title,NULL,str,NULL,NULL,NULL,0);
	 va_end(arglist);

}

flag_t Write_Header(FILE *fpt,short total=0, short selection=0)
{
	flag_t ret = NO_ERR;
	list_header_t header;
	header.total = total;
	header.selection=selection;

		 if (fseek(fpt,0L,SEEK_SET) != 0)
			ret = ERROR;
		 else
			{
			 if (fwrite(&header, sizeof(list_header_t), 1, fpt) != 1)
				{
				 printf("Error writing to file!");
				 ret = ERROR;
				}
			}
		 return(ret);
}

flag_t Save_Combo_Item_Pos(const char *fname,const char **items,short num,char pos=1)
{
	FILE *fp=NULL;
	short int total,i;
	flag_t ret=0;
	list_t combo_list;
	list_header_t header;
	fp = fopen(fname,"r+b");  //read+write existing file
		if (fp == NULL)
		{
			fp = fopen (fname,"w+b"); //read+write new file
			if (fp == NULL)
			{
				printf ("File open error:%s\n",fname);
				return ERROR;
			}
			if (Write_Header(fp))
				return ERROR;
		}

	if (fseek(fp,0L,SEEK_SET) != 0)
			return ERROR;
	if (fread(&header,sizeof(list_header_t),1,fp) != 1)  //read the header
		{
			printf ("Invalid Header in file\n");
			return ERROR;
		}
	total = header.total;

	//go to file end
	while(!feof(fp))
		getc(fp);
	for (i=0;i<num;i++)
	{
		strcpy(combo_list.item,items[i]);
		if(fwrite(&combo_list,sizeof(list_t),1,fp) != 1)
			break;
		total++;
	}

	ret=Write_Header(fp,total,pos);
	fclose(fp);
	return ret;


}

flag_t Load_Combo_From_File(PtWidget_t *widget,const char *fname,const char *in_tag)
{
	FILE *fp=NULL;
	char found=0,*pt=NULL,tag[20],ret=NO_ERR,line[30];
	char item[30];
	const char *address[]={item};
	short line_num=0,i;
	int num=0,sel_index=0;
	fp = fopen(fname,"r");
	if (fp == NULL)
	{
		printf("File read error:%s\n",fname);
		return ERROR;
	}
	while(found == FALSE)
	{
		if(feof(fp))
			break;
		//remove the trailing new line character

		if(fgets(line,50,fp) == NULL)
			break;
		line_num++;

		if ((pt =strchr(line,'@')) == NULL)
			continue;
	    if(sscanf(pt+1,"%s %d %d",tag,&num,&sel_index) != 3)
	    {
	    	printf ("Combo file:%s Error at line:%d\n",fname,line_num);
	    	ret = ERROR;
	    	break;
	    }
	    if (strcmp(in_tag,tag) == 0)
	      found =TRUE;
	}
	if (!found)
	  {
		printf ("Combo File:%s Error!, tag:%s not found\n",fname,in_tag);
		ret = ERROR;
	  }

	if (num> COMBO_LIST_MAX)
		num=COMBO_LIST_MAX;
	if(sel_index > num)
		sel_index = num;
	for(i=0;(i<num) && (ret == NO_ERR);i++)
	{
		if (fgets(item,30,fp)== NULL)
			break;
		if((pt = strchr(item,'\n')) !=NULL)
			*pt='\0';
		if(PtListItemExists( widget,(const char *)item ))  //already exists
			continue;
		if(PtListAddItems(widget,address,1,0))//add 1 item at end
			ret= ERROR;
	}
	if(!ret)
		 PtSetResource(widget,Pt_ARG_CBOX_SEL_ITEM,sel_index,0);
	fclose(fp);
	return ret;
}

flag_t Load_Combo_From_File(PtWidget_t *widget,const char *fname)
{
	FILE *fp=NULL;
	char found=0,*pt=NULL,tag[20],ret=NO_ERR,line[30];
	char item[30],in_tag[20];
	const char *address[]={item};
	short line_num=0,i;
	int num=0,sel_index=0;

	if(!Get_User_Data(widget,in_tag))
		return ERROR;

    //debug
	//printf("Tag:%s\n",in_tag);
	fp = fopen(fname,"r");
	if (fp == NULL)
	{
		printf("File read error:%s\n",fname);
		return ERROR;
	}
	while(found == FALSE)
	{
		if(feof(fp))
			break;
		//remove the trailing new line character

		if(fgets(line,50,fp) == NULL)
			break;
		line_num++;

		if ((pt =strchr(line,'@')) == NULL)
			continue;
	    if(sscanf(pt+1,"%s %d %d",tag,&num,&sel_index) != 3)
	    {
	    	fprintf (stderr,"Combo file:%s Error at line:%d\n",fname,line_num);
	    	/*
	    	ret = ERROR;
	    	break;
	    	*/
	    	continue;  //anyway proceed
	    }
	    if (strcmp(in_tag,tag) == 0)
	      found =TRUE;
	}
	if (!found)
	  {
		printf ("Combo File:%s Error!, tag:%s not found\n",fname,in_tag);
		ret = ERROR;
	  }

	if (num> COMBO_LIST_MAX)
		num=COMBO_LIST_MAX;
	if(sel_index > num)
		sel_index = num;
	for(i=0;(i<num) && (ret == NO_ERR);i++)
	{
		if (fgets(item,30,fp)== NULL)
			break;
		if((pt = strchr(item,'\n')) !=NULL)
			*pt='\0';
		if(PtListItemExists( widget,(const char *)item ))  //already exists
			continue;
		if(PtListAddItems(widget,address,1,0))             //add 1 item at end
			ret= ERROR;
	}
	if(!ret)
		 PtSetResource(widget,Pt_ARG_CBOX_SEL_ITEM,sel_index,0);
	fclose(fp);
	return ret;
}


flag_t Save_Combo_To_File(PtWidget_t *widget, const char *fname)
{
	flag_t ret = NO_ERR;
	FILE *fp=NULL;
	char **adr,new_text[30],*new_pt[]={new_text};
	unsigned short *len=NULL,sel_item,i;
	char tag[20];
	fp = fopen(fname,"a");
	if (fp == NULL)
	{
		printf("File open error:%s\n",fname);
		return ERROR;
	}

	if(!Get_User_Data(widget,tag))
		return ERROR;

	if (Get_Text(widget,new_text) == -1)   //get the current combo text
		{
		printf ("Error getting text from combo!\n");
		return ERROR;
		}

	//check user has entered any fresh values
	if(!PtListItemExists( widget,(const char *)new_text ))  //not an existing item
	  {
		if(PtListAddItems(widget,(const char **)new_pt,1,1))//add 1 item at end
		  return ERROR;
		sel_item=1;
	  }
	else
		//determine the pos of the selected item
		sel_item = PtListItemPos(widget,(const char*)new_text);

	PtArg_t          args;
	PtSetArg( &args, Pt_ARG_ITEMS, &adr, &len );  //get the list of items and total list count
	if(PtGetResources(widget,1,&args))
		{
		fclose(fp);
		return ERROR;
		}

	fprintf(fp,"@%s %d %d\n",tag,(int)*len,(int)sel_item);
    for (i=0;i<*len;i++)
      	fprintf(fp,"%s\n",adr[i]);
    fclose(fp);
	return ret;
}

flag_t Load_Combo_File(PtWidget_t *widget,const char *fname)
{
	FILE *fp=NULL;
	flag_t ret=0;
	list_t combo_list;
	list_header_t header;
	//unsigned int val=0;
	unsigned short i;

	char *val_ptr[]={combo_list.item};
	if (widget == NULL)
		return ERROR;
	fp = fopen (fname,"rb");
	if (fp == NULL)
	{
		printf ("Combo File Not Found:%s",fname);
		return NO_ERR;  //may be first time
	}
	if (fread(&header,sizeof(list_header_t),1,fp) != 1)  //read the header
	{
		printf ("Invalid Header in file:%s\n",fname);
		return ERROR;
	}

	for (i=0;i<header.total &&!ret;i++)
	{
		if (fread(&combo_list,sizeof(list_t),1,fp) != 1)
		{
			ret= TRUE;
			break;
		}
		if(PtListItemExists( widget,(const char *)combo_list.item ))  //already exists
			continue;

		if(PtListAddItems(widget,(const char **)val_ptr,1,1))//add 1 item at the begening
			                                                 //val_ptr points to the data
			ret=TRUE;

	}
	fclose(fp);

	if(!ret)  //set the selection
	 PtSetResource(widget,Pt_ARG_CBOX_SEL_ITEM,header.selection,0);
	return ret;
}

flag_t Save_Combo_Selection(PtWidget_t *widget,const char *fname)
{
	short *sel;
	PtGetResource(widget,Pt_ARG_CBOX_SEL_ITEM,&sel,0);
	return Save_Combo_Item_Pos(fname,NULL,0,*sel);
}

flag_t Add_Combo_Items(PtWidget_t *widget,const char **items,short num,char sel_index)
//default position is at start
{
	char pos=1;
	short i;
	//const char *items[]={item};
	if (!widget)
		return ERROR;
	for (i=0;i<num;i++)
	{
	if(PtListItemExists( widget,items[i] ))  //already exists
		continue;
	if(PtListAddItems(widget,items+i,1,pos))//add 1 item at pos
		return ERROR;
	}
	 PtSetResource(widget,Pt_ARG_CBOX_SEL_ITEM,sel_index,0);
	  return NO_ERR;
	//return Save_Combo_Item_Pos(fname,items,num,pos);

}

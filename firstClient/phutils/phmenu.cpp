/*
 * phmenu.cpp
 *
 *  Created on: Jan 3, 2011
 *      Author: Aneesh.K.Thampi
 *      menu ver1.1 26/04/11
 *       Menu Title display is through Arg_MENU_TITLE_TEXT property (No button)
 *       Menu conf file now requires legend to be enclosed within quotes
 *       08/11/11
 *          Show_Menu
 *          Position_Menu ( argument changed to cbinfo->event) to position menu attcahed non-menubutton widgets
 */
#include "phmenu.hpp"
#include "common_utils.hpp"
#include <string.h>


Ph_Menu::Ph_Menu(  )
{
	sint i;
	status = NOT_OK;
	Widget_Buffer = NULL;
	menu_level =0;
	Setup_CB_Done = FALSE;
	Tag = NULL;
	widget_buf_cnt=0;
	Widget_Count=0;
	for(i=0;i<MAX_MENU_LEVEL;i++)
		Menu_Widget[i] = NULL;


}

Ph_Menu::~Ph_Menu()
{

	Destroy();

}
void Ph_Menu::Detach()
{
	Destroy();
}

void Ph_Menu::Destroy()
{
	sint i;
	if(status == OK)
		{
		if( Setup_CB_Done== TRUE)  //if already attached, remove menu setup function
			PtRemoveCallback(Root_Parent,Pt_CB_ARM,Show_Menu,Main_Menu);
		     /*
		      * Root_Parent and Main_Menu saved During call to Init()
		      */
		Setup_CB_Done=FALSE;

		//destroy menu button widgets
			for (i=0;i<Widget_Count;i++)
			{
				if(PtDestroyWidget(Widget_Buffer[i]) == -1)
							printf("Error Destroying Menu Item Widget\n");
				//Widget_Buffer[i] = NULL;
		     }
			Widget_Count =0;
		}

	//distroy menu widgets
	for(i=0;i<menu_level;i++)
		if(PtDestroyWidget(Menu_Widget[i]) == -1)
		  printf("Error Destroying Menu Widget\n");
	menu_level =0;


	if(Widget_Buffer!=NULL)
			delete []Widget_Buffer;  //deallocate the widget buffer
		if(Tag)
			delete []Tag;
	Widget_Buffer = NULL;
	Tag = NULL;
	status = NOT_OK;
	return;
		//printf("memory released!");

}

flag_t Ph_Menu::Init(PtWidget_t *Menu_Parent,const char *fname,int (*menu_callback)(PtWidget_t *,void *,PtCallbackInfo_t *))
{
	short i=0;


	//destroy menu if already created
	if(status == OK)
	  Destroy();
	status = NOT_OK;

	fp = fopen(fname,"r");
	if ( fp == NULL)
	{
		printf ("Could not open Menu File:%s!",fname);
		return NOT_OK;
	}

	widget_buf_cnt = Get_Widget_Count();
	if(widget_buf_cnt == 0)
		{
		printf("Error in Menu File:%s\n",fname); //erraneous file
		return NOT_OK;
		}

	try
	{
	Widget_Buffer = new PtWidget_t*[widget_buf_cnt];
	}
	catch(bad_alloc)
	{
		printf("Widget Memory Allocation error");
		status = NOT_OK;
		return NOT_OK;
	}

	try
	{
		Tag = new tag_t[widget_buf_cnt];
	}
	catch(bad_alloc)
	{
		printf("Widget Tag Memory allocaion Error");
		status = NOT_OK;
		return NOT_OK;
	}

	//Initialise all tags to null
	//sublinks may not have tags but they are widgets stored in the buffer
	//tag corresponding to them should not be left uninitialized since we will travers thru this array when disabling an item
    for (i=0;i<widget_buf_cnt;i++)
    	*Tag[i].text='\0';


	line_num=0;

	if((Main_Menu = Generate_Menu_Items(Menu_Parent,menu_callback)) == NULL)
		{
		//error
		printf("Menu File:%s\n",fname);
		return NOT_OK;
		}
	g_callback = menu_callback;  //save the pointer to remove the call back later

	//add menu set up call back to menu parent

	PtAddCallback(Menu_Parent,Pt_CB_ARM,Show_Menu,Main_Menu);
	Setup_CB_Done = TRUE;
	Root_Parent = Menu_Parent;  //save it to remove the call back in destroy()

	status = OK;
	fclose(fp);
	return OK;
}

/*
 * Callback function for menu parent
 * It Assumes menu already created
 */

int Show_Menu(PtWidget_t *widget,void *cbdata,PtCallbackInfo_t *cbinfo)
{
	widget=widget,cbdata=cbdata,cbinfo=cbinfo ;
	if(cbdata)
	{
	PtWidget_t *menu = (PtWidget_t*)cbdata;

	PtPositionMenu(menu,cbinfo->event);
	PtRealizeWidget(menu);
	}
	return Pt_CONTINUE;
}

/*
 * Generate_Menu_Items()
 * To read the menu file generate the menu items
 * First argument is the parent widget to which menu is to be attached
 * Second arguement: global callbackfunction (where event receives when menu item is clicked)
 * It returns a pointer to the created menu item.
 *         File opened using fp
 *
 */
PtWidget_t *Ph_Menu::Generate_Menu_Items(PtWidget_t *parent_widget,int (*global_callback)(PtWidget_t *widget,void *apinfo,PtCallbackInfo_t *cbinfo))
{

	//PtWidget_t *parent_wid_level1,*parent_wid_level2,*parent_wid_level3,*parent_wid_level4,*Cur_Widget=NULL,*Cur_Parent;
	PtWidget_t *Cur_Parent;

	char Helvetica_14B[MAX_FONT_TAG];
	//byte sub_menu_level=0;

	char tag[10],type,enabled;
	char text[MAX_MENU_TEXT];
	int argc=0;
	//flag_t ret=OK;

	PtArg_t arg[8];

	if(menu_level == MAX_MENU_LEVEL)
	{
		cerr<<"Maximum menu level reached!. Error\n";
		cerr<<"line num:"<<line_num<<"\n";
		return NULL;
	}

	//create the menu
	//PtSetArg(&arg[argc++],Pt_ARG_MENU_ITEM_FILL_COLOR,Pg_TRANSPARENT,0);


	if (Menu_Attr & SEL_COLOR)  //color to indicate selection
		{
		PtSetArg(&arg[argc++],Pt_ARG_MENU_ITEM_HIGHLIGHT_COLOR,Sel_Color,0);
		PtSetArg(&arg[argc++],Pt_ARG_SUBMENU_PARENT_HIGHLIGHT_COLOR,Sel_Color,0);
		}

	if (Menu_Attr & SET_FONT)
		PtSetArg(&arg[argc++],Pt_ARG_MENU_TEXT_FONT,User_Def_Font,0);

	if(menu_level == 0)    //if the main menu widget
	{
		PtSetArg(&arg[argc++], Pt_ARG_MENU_FLAGS,Pt_MENU_TEAR_OFF,Pt_MENU_TEAR_OFF);
		Menu_Widget[menu_level] = PtCreateWidget(PtMenu,parent_widget,argc,arg);

	}
	else                 //sub level menus,display arrow
	 {
		PtSetArg(&arg[argc++], Pt_ARG_MENU_FLAGS,Pt_MENU_AUTO|Pt_MENU_CHILD,Pt_MENU_AUTO|Pt_MENU_CHILD);
		Menu_Widget[menu_level] = PtCreateWidget(PtMenu,parent_widget,argc,arg);

	 }
	if (Menu_Widget[menu_level] == NULL)
	 {
			printf("Error Creating Menu Widget\n");
			return NULL;
	 }


	Cur_Parent = Menu_Widget[menu_level];
	if(PfGenerateFontName("TextFont",PF_STYLE_BOLD , 9,   //PF_STYLE_ITALIC|PF_STYLE_BOLD
	                        Helvetica_14B) == NULL) {
	      perror("Unable to generate font name");
	      return NULL;
	   }

	argc=0;
	while(fgets(line,LINE_MAX,fp) != NULL)
		{
		line_num ++;

		if( Get_Item_Details(text,&type,tag,&enabled))
		 {
			printf("Error at line:%d ",line_num);
			return NULL;
		 }

		if(type == INVAL)
			continue;
		argc=0;
		PtSetArg(&arg[argc++], Pt_ARG_TEXT_STRING, text, 0);

		if(enabled == FALSE)  //disabled item
			PtSetArg(&arg[argc++],Pt_ARG_FLAGS,Pt_FALSE,Pt_SELECTABLE);


		if (Menu_Attr & BACK_COLOR)
			PtSetArg(&arg[argc++], Pt_ARG_FILL_COLOR,Back_Color,0);
		else
			PtSetArg(&arg[argc++], Pt_ARG_FILL_COLOR,Pg_TRANSPARENT,0);


		switch(type)
		{
		case SUB_LINK:  //no call back function
			PtSetArg(&arg[argc++], Pt_ARG_BUTTON_TYPE, Pt_MENU_RIGHT,Pt_MENU_RIGHT);
			PtSetArg(&arg[argc++],Pt_ARG_HEIGHT,17,0);
			PtSetArg(&arg[argc++], Pt_ARG_HORIZONTAL_ALIGNMENT, Pt_CENTER, 0);
			Widget_Buffer[Widget_Count++] = PtCreateWidget(PtMenuButton, Cur_Parent, argc, arg);
			menu_level++;
		    strcpy(Tag[Widget_Count-1].text,tag);
			Generate_Menu_Items(Widget_Buffer[Widget_Count-1],global_callback);
			break;
		case TITLE: //no tag,nocallback,unselectable
			argc=0;
			PtSetArg(&arg[argc++], Pt_ARG_MENU_TITLE, text, 0);
			PtSetArg(&arg[argc++], Pt_ARG_MENU_TITLE_FONT, Helvetica_14B, 0);
			PtSetResources( Cur_Parent,argc,arg );
			break;
		case SEP:
			PtCreateWidget(PtSeparator,Cur_Parent,0,NULL);
			break;
		case NORMAL:
			//PtSetArg(&arg[argc++], Pt_ARG_HORIZONTAL_ALIGNMENT, Pt_LEFT, 0);
			Widget_Buffer[Widget_Count++] = PtCreateWidget(PtButton, Cur_Parent, argc, arg);
			strcpy(Tag[Widget_Count-1].text,tag);
			PtAddCallback(Widget_Buffer[Widget_Count-1],Pt_CB_ACTIVATE,global_callback,(Tag[Widget_Count-1].text));
			break;
		case GOBACK:
			menu_level--;
		case FEND:

			return Cur_Parent;
		}


     }



	return Menu_Widget[menu_level];   //may not execute
}

flag_t Ph_Menu ::Set_Font(char const *fpackage,unsigned long flags,unsigned long fsize )
{
	if(PfGenerateFontName(fpackage,flags, fsize,User_Def_Font) == NULL)
	{
	   perror("Unable to generate font name");
	   return ERROR;
	}
	Menu_Attr |= SET_FONT;
	return NO_ERR;

}

void Ph_Menu ::Set_Color(PgColor_t back_color,PgColor_t fcolor,PgColor_t sel_color)
{
	Back_Color = back_color;
	Menu_Attr |= BACK_COLOR;

	Font_Color = fcolor;
	if (Font_Color)
		Menu_Attr |= FONT_COLOR;

	Sel_Color  = sel_color;
	if(Sel_Color)
		Menu_Attr |= SEL_COLOR;
	return;

}

flag_t Ph_Menu::Get_Item_Details(char *text,char *type,char *tag,char *enabled)
{
	char cur_line[MAX_LINE];
	flag_t ret=0,offset=0;
	strcpy(cur_line,line);
	char *ch=cur_line;
    *text='\0';


	char *pt;
	sint cnt=0;
	*enabled = TRUE;  //default enabled
	*tag = '\0';
	if((ch = strchr(cur_line,';')) != NULL)
		*ch = '\0';

	if((ch = strchr(cur_line,'<')) != NULL)
	 {
		*type = GOBACK;
		return OK;       //no other entries expected here, so return
	 }
	if((ch = strchr(cur_line,'#')) != NULL) //end of file
	{
	*type = FEND;
	return OK;
	}

	if ((ch = strchr(cur_line,'>')) != NULL)
	{
		*type = SUB_LINK;
		ch++;
	}
	else if ((ch = strchr(cur_line,'^')) != NULL)
	{
		*type = TITLE;
		ch++;
	}
	else if(cur_line[0] == '-')
	{
		*type = SEP;
		return OK;
	}

	else
		{
		*type = NORMAL;
		ch = cur_line;
		}

	//ch = strchr(cur_line,'\"')+1;
	/*

	pt = strtok(ch," \t\n\r");
	if(pt == NULL)
	{
		if(*type == NORMAL)   //may be a blank line
		 {
		  *type = INVALID;
		  return OK;
		 }
	   else
		return NOT_OK;  //text is a must entry
	}

*/
	ret = Get_String_Token(ch,text,'"',&offset);  //search for string enclosed in "",ie menu item name
    if (ret) //error
    {
    	if(*type == NORMAL)   //may be a blank line
    			 {
    			  *type = INVAL;
    			  return OK;
    			 }
    		   else
    			return NOT_OK;  //text is a must entry

    }
    ch+=offset;

	//strcpy(text,pt);
	cnt++;

	pt = strtok(ch," \t\n\r");
	if( pt!=NULL)
	{
		strcpy(tag,pt);
		cnt++;
		if((pt = strtok(NULL,"\t \r\n")) != NULL)
		{
			if((*pt == 'D')||(*pt == 'd'))
				*enabled = FALSE;
		}

	}

	if(*type == NORMAL && cnt ==1)  //for normal menu item 'tag' is a must
		return NOT_OK;


	return OK;

}
usint Ph_Menu::Get_Widget_Count()
{
	usint cnt=0;

	while(fgets(line,LINE_MAX,fp) != NULL)
	{
		if(!strlen(line))
			continue;
		if(strchr(line,'<') != NULL)
			continue;
		if(strchr(line,'^') != NULL)  //title
			continue;
		if(line[0] == '-')   //separator
			continue;
		if(strchr(line,'#') != NULL)  //end of file
			break;
		cnt++;
	}
	rewind(fp);
//	printf("Total widgets:%d",cnt);
	if(!cnt)
		printf("No Items found!");
	return cnt;

}

flag_t Ph_Menu ::Disable_Item(const char *tag)
{
	sint i;
	if(status != OK)
		return NOT_OK;

	for(i=0;i<widget_buf_cnt;i++)
		if(strcmp(tag,Tag[i].text) == 0)
			PtSetResource(Widget_Buffer[i],Pt_ARG_FLAGS,Pt_FALSE,Pt_SELECTABLE);

	return OK;



}

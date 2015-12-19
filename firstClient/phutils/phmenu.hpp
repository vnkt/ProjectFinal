/*
 * phmenu.hpp
 *
 *  Created on: Jan 3, 2011
 *      Author: Aneesh.K.Thampi
 */

#ifndef PHMENU_HPP_
#define PHMENU_HPP_
#include <Pt.h>
#include <Ph.h>
#include <Ap.h>
#include <photon/Pg.h>
#include <photon/pf.h>
#include <stdio.h>
#include <iostream.h>
#include "dtypes.h"

#define MAX_MENU_LEVEL 5
#define MAX_MENU_TEXT 25
#define MAX_MENU_ITEMS 25
#define TAG_LEN 5


#define BACK_COLOR 0x1
#define FONT_COLOR 0x2
#define SEL_COLOR  0x4
#define SET_FONT   0x8

#define BOLD PF_STYLE_BOLD
#define ITALIC PF_STYLE_ITALIC


typedef struct
{
	char text[TAG_LEN];
}tag_t;


//callback function for all menu click events
int  Menu_Callback(PtWidget_t *widget,void *data, PtCallbackInfo_t *cbinfo );

enum{NORMAL,SUB_LINK,TITLE,SEP,GOBACK,FEND,INVAL};

int Show_Menu(PtWidget_t *,void *,PtCallbackInfo_t *);  //call back for menu parent

class Ph_Menu
{

private:
	char status;
	unsigned char Menu_Attr;
	char Setup_CB_Done;
	char line[MAX_LINE];
	sint line_num;
	FILE *fp;
	PtWidget_t *Main_Menu,*Root_Parent;
	PtWidget_t **Widget_Buffer;
	PtWidget_t *Menu_Widget[MAX_MENU_LEVEL];
	sint widget_buf_cnt;
	sint menu_level;
	sint Widget_Count;
	char User_Def_Font[MAX_FONT_TAG];
	PgColor_t Back_Color,Font_Color,Sel_Color,User_Def_Color;
	//sint menu_count;
	tag_t *Tag;
	//char Tag[10];
	int (*g_callback)(PtWidget_t *,void *,PtCallbackInfo_t *);


	//methods
	usint Get_Widget_Count ();

    //call back function for the menu
    int Menu_Callback(PtWidget_t *widget,void *data, PtCallbackInfo_t *cbinfo );



    PtWidget_t * Generate_Menu_Items(PtWidget_t *parent_widget,int (*global_callback)(PtWidget_t *widget,void *apinfo,PtCallbackInfo_t *cbinfo));
    flag_t Get_Item_Details(char *text,char *type,char *tag,char *enabled);
    void Destroy();


public:

	//char Tag[MAX_MENU_ITEMS][10];


	//constructor
	Ph_Menu();
	~Ph_Menu();
	void Detach();
	flag_t Disable_Item(const char *tag);
	flag_t Set_Font(char const *fpackage,unsigned long flags,unsigned long fsize );// PF_STYLE_ITALIC|PF_STYLE_BOLD
	void Set_Color(PgColor_t back_color,PgColor_t fcolor=0,PgColor_t sel_color=0);


	//initialize the pointer
	//it takes menu parent pointer ,menu filename,global callback function as arguments
    flag_t Init(PtWidget_t *widptr,const char *fname,int (*menu_callback)(PtWidget_t *,void *,PtCallbackInfo_t *));





};
#endif /* PHMENU_HPP_ */

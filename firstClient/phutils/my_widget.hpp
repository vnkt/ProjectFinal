/*
 * my_widget.hpp
 *
 *  Created on: Feb 28, 2011
 *      Author: Administrator
 */

#ifndef MY_WIDGET_HPP_
#define MY_WIDGET_HPP_
#include <Pt.h>
#include <Ph.h>
#include <photon/Pg.h>
#include <Ap.h>
#include "dtypes.h"

//Picture definitions as per the picture module designed in phab
#define PH_PICT_PROJECT 0
#define PH_PICT_NORMAL 1
#define PH_PICT_INFO 2
#define PH_PICT_CRITICAL 3
#define PH_PICT_PASSWORD 4
#define PH_PICT_SUCCESS 5
#define PH_PICT_BYPASS 6

#define PH_NORMAL PH_PICT_NORMAL
#define PH_INFO PH_PICT_INFO
#define PH_CRITICAL PH_PICT_CRITICAL
#define PH_SUCCESS PH_PICT_SUCCESS



#define CHECK_LIMIT     0x1
#define FLOAT_VALUE     0x2
#define INTEGER_VALUE   0x4
#define LIMIT_EXCEEDED  0x8

#define MAX_LIST_LENGTH 50
#define COMBO_LIST_MAX 10

//pictures stored in the picture database
//store the picture name strings in the same order.Eg MyDialog.GetPicture()
//update both enum and picture list
enum pict_ind{BULB1,BULB2};

//miscellaneous
short   Get_Text (PtWidget_t *wid,char *ret_text = NULL );
short   int Get_Text_Ptr(PtWidget_t *wid,char *buf );
void    Set_Color_Text(PtWidget_t *wid,PgColor_t Color,const char *fmt,...);
char    Get_Font (PtWidget_t *wid,char *ret_font);
flag_t  Get_Numeric_Int (PtWidget_t *wid,long int * val);
flag_t  Get_Numeric_Int (PtWidget_t *wid,int * val);
void    Set_Numeric_Int (PtWidget_t *wid,long int val);
flag_t    Get_User_Data(PtWidget_t *wid,char *data);
void    Set_Numeric_Float(PtWidget_t *wid,double val);
//void    Set_Numeric_Float(PtWidget_t *wid,float val);
void    Set_Numeric_Int(PtWidget_t *wid, long int val);

flag_t  Get_Numeric_Float(PtWidget_t *wid,float *val);
flag_t  Get_Numeric_Float(PtWidget_t *wid,double *val);
flag_t Get_List_Index(PtWidget_t *wid);


void    Hide_Widget(PtWidget_t *wid);
flag_t  Check_Widget_Selection(PtWidget_t *wid);
void    Enable_Disable_Widget (PtWidget_t *wid,flag_t flg);
void Set_Widget_State (PtWidget_t *wid,flag_t flg=TRUE);
void Hide_Window(PtWidget_t *wid);
void Show_Window(PtWidget_t *wid);
void    Set_Window_Title(PtWidget_t *wid,const char *str);


//Combo box list related

typedef struct
{
	char item[MAX_LIST_LENGTH];
}list_t;
typedef struct
{
	short total;
	short selection;
}list_header_t;
flag_t Load_Combo_File(PtWidget_t *widget,const char *fname);
flag_t Load_Combo_From_File(PtWidget_t *widget,const char *fname,const char *tag);
flag_t Load_Combo_From_File(PtWidget_t *widget,const char *fname);
flag_t Save_Combo_To_File(PtWidget_t *widget, const char *fname);//,const char *tag);
flag_t Add_Combo_Items(PtWidget_t *widget,const char **item,short num=1,char sel_index=1);
flag_t Save_Combo_Selection(PtWidget_t *widget,const char *fname);

//Callback related
typedef struct
{
	PtWidget_t *wid_ptr;
	const char *tag;

}Callback_List_t;

void Add_Callback(Callback_List_t *list,short count,int (*callback)(PtWidget_t *widget,void *apinfo,PtCallbackInfo_t *cbinfo));


// display data in widgets with limit check

typedef struct {
	long int max;
	long int min;
}int_limit_t;
typedef struct {
	double max;
	double min;
}float_limit_t;
typedef union
{
	int_limit_t   int_limit;
	float_limit_t float_limit;
}limit_t;

class My_Widget
{
private:
	PtWidget_t *wid_ptr;
	flag_t     limit_status;
	limit_t    limit;
	char str[51];
	PgColor_t Err_Color,Org_Color,*Color_Ptr;
	void Set_Limit_Hit(flag_t status);

public:
	           My_Widget  (PtWidget_t *wid,const int min,const int max,PgColor_t Color=Pg_RED);
	           My_Widget  (PtWidget_t *wid,const double min,const double max,PgColor_t Color=Pg_RED);
	           My_Widget  (PtWidget_t *wid);
	short      Get_Data(char *text);
	flag_t     Get_Data(long int *val);
	flag_t     Get_Data(int *val);
	flag_t     Get_Data(float *val);
	flag_t     Get_Data(double *val);


	void       Set_Data(const int val);
	void       Set_Data(const double val);
	void       Set_Text_chk(const char *fmt,const double val);
	void       Set_Text_chk(const char *fmt,const int val);
	void       Set_Text(const char *fmt,...);
	void       Set_Color_Text(PgColor_t Color,const char *fmt,...);

};


#define PH_OK_ONLY 0
#define PH_OK_CANCEL 1
#define PH_YES_NO 2
#define PH_YES_NO_CANCEL 3

#define PH_OK 1
#define PH_CANCEL 3
#define PH_YES 1
#define PH_NO 2




#define MAX_BUF 25

//class for commonly used Dialogs
class My_Dialog
{
private:
	ApDBase_t *dbase;
	PtWidget_t *parent;
	PhImage_t *image_ptr;
	char buf[MAX_BUF+1];
	char font_text[MAX_FONT_TAG],*font;

	flag_t Get_Input(const char *title,const char* msg,char *data,int cnt);
	//PhImage_t image;
public:
	My_Dialog(){ dbase=NULL;parent=NULL;image_ptr=NULL;font=NULL;};

	//here specify the picture module instance to display images
	//My_Dialog(ApEventLink_t const *link_instance){dbase = ApOpenDBase(link_instance);parent=NULL;font=NULL;};
	//My_Dialog(const char *db_filename);
	~My_Dialog(){if (dbase)	ApCloseDBase(dbase);};

	flag_t Init(ApEventLink_t const *link_instance);

	void   Set_Parent(PtWidget_t *wid){parent = wid;};
	flag_t   Set_Font(char const *pkge,unsigned int flg,unsigned int size);
	flag_t Get_Picture(flag_t type);

	//inputs
	flag_t Get_Int(int &val,const char *msg);
	flag_t Get_Float(float &val,const char *msg,const char *fmt="%f");
	flag_t Get_Text(char *val,const char *msg,short len);

	//message display
	flag_t msgbox(const char *msg,const char *title,flag_t type,flag_t def_button,flag_t pict=0);
	flag_t msgbox(const char *title,flag_t type,flag_t def_button,flag_t pict,const char *fmt,...); //avriable argument version
	flag_t msgbox(const char *msg,const char *title,flag_t pict,const char *buttons[],short int btcount,flag_t def_button); //msgbox with user defined buttons
	//flag_t dis_msg(const char *msg,const char *title,flag_t type,const char *btntext="&OK");
	void Dis_Msg(const char *title,flag_t pict,const char *fmt,...);
	void Dis_Msg_Wait(const char *title,flag_t pict,const char *fmt,...);
	//password prompt
	flag_t Chk_Password(const char *exp_password,const char *fmt...);

};

//Common dialogues without class
void Dis_Msg(PtWidget_t *parent,const char *title,const char *fmt,...);
#endif /* MY_WIDGET_HPP_ */

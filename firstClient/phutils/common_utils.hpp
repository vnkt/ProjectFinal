/*
 * common_utils.hpp
 *
 *  Created on: Mar 12, 2011
 *      Author: Aneesh K Thampi
 */

#ifndef COMMON_UTILS_HPP_
#define COMMON_UTILS_HPP_
#include <Pt.h>
#include <Ph.h>
#include <photon/Pg.h>
#include <Ap.h>

#include "dtypes.h"


flag_t Get_String_Token(const char *in_string,char *out_string,char delim,flag_t *offset=NULL);
//To save or get a value to a file
short Get_Sav_Val_File(const char *fname,short val=INVALID);

//To get the index of the given widget pointer in a given array
//To identify the widget in the callback

flag_t Get_Widget_Index(PtWidget_t *wid,int *array,flag_t num);

//To block a window (!!caution its not thread safe)
flag_t Block_Window(PtWidget_t *wnd,flag_t block);


//________Main Panel Display Class______


#define MAX_ROWS 50
#define MAX_COLUMN 20 //40
#define DISPLAY_BOLD 0x1
#define DISPLAY_ITALIC 0x2

//text widget


// main display showing checkout activities
class Panel_Display
{
private :
	PtWidget_t *ptr;
//	char col_length[MAX_ROWS];
	unsigned short int curr_row,start,end,length;

	char font[MAX_FONT_TAG];
	int mask;
	PtMultiTextAttributes_t attr;
	char dis_buf[(MAX_ROWS) * (MAX_COLUMN+1) +1];

	flag_t Update_Index(const char *str,char in_row=0,char in_col=0);
	void   Modify_Single_Char(char c);
	void   Set_Font_Attr(PgColor_t color,PgColor_t back_color,char font_attr=0);

public:
	Panel_Display ();
	void Dis      (const char *str,char in_row=0,char in_col=1,PgColor_t color=0,PgColor_t back_color = 0,char font_attr=0);
	void wprintf  (char row,char col,const char *fmt,...);
	void wgprintf  (char row,char col,PgColor_t color,PgColor_t back_color,char font_attr,const char *fmt,...); //with font graphics
	void Clear    (char in_row= 1, char in_col =1);
	void Init     ( PtWidget_t *wid);
};


#endif /* COMMON_UTILS_HPP_ */

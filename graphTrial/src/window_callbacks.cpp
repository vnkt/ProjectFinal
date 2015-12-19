/* Y o u r   D e s c r i p t i o n                       */
/*                            AppBuilder Photon Code Lib */
/*                                         Version 2.03  */

/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/neutrino.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>

/* Local headers */
#include "ablibs.h"
#include "abimport.h"
#include "proto.h"
#include "point.hpp"

typedef union
{
	struct _pulse   pulse;
} my_message_t;

graph points(21, 228);
point_det* plot_point;
point_det prev_point, curr_point;
int num_points;
name_attach_t *timer_server;
char fifo_name[20];
int col_num;

void abc( PtWidget_t *widget, PhTile_t *damage )
{
	PhRect_t rect;
	damage = damage;
	PtSuperClassDraw( PtBasic, widget, damage );
	PtCalcCanvas( widget, &rect );
	PtClipAdd( widget, &rect );

	int x_axis = points.get_x_axis();
	int y_axis = points.get_y_axis();
	int width = points.get_width_axis();
	int height = points.get_height_axis();
	float *x_points, *y_points;
	float *x_points_val , *y_points_val;
	char temp[10];
	PhPoint_t p;
	char font_name[MAX_FONT_TAG];

	plot_point = points.get_points();
	num_points = points.no_points();
	x_points = points.get_x_point();
	y_points = points.get_y_point();
	x_points_val = points.get_x_point_pos();
	y_points_val = points.get_y_point_pos();

	PgSetStrokeColor( Pg_BLACK );
	PgSetTextColor( Pg_BLACK );
	PgSetStrokeWidth( 2 );
	PgDrawILine( 0, y_axis, width + x_axis, y_axis);
	PgDrawILine( x_axis, 0 , x_axis, height + y_axis );

	unsigned char dash[5];
	memcpy(dash, "\1", 2);
	PgSetStrokeWidth( 1 );
	PgSetStrokeDashCx(PgGetGC(), dash, 1, 0);
	if (PfGenerateFontName( "Helvetica", PF_STYLE_BOLD, 10, font_name ) != NULL )
	   PgSetFont( font_name );
	p.y = height - 10;
	for(int i = 1 ; i < 13; i++)
	{
		sprintf(temp, "%0.1f", x_points[i]);
		p.x = x_points_val[i];
		PgDrawText( temp, strlen( temp ), &p, Pg_TEXT_RIGHT);
		PgDrawILine( p.x, 16 , p.x, height + y_axis );
	}

	p.x = x_axis;
	for(int i = 0 ; i < 12; i++)
	{
		sprintf(temp, "%0.3f", y_points[i]);
		p.y = y_points_val[i];
		if(i < 6)
			PgDrawText( temp, strlen( temp ), &p, Pg_TEXT_TOP );
		else
			PgDrawText( temp, strlen( temp ), &p, Pg_TEXT_BOTTOM );
		PgDrawILine( 23, p.y, width + x_axis, p.y);
	}

	PgSetStrokeDashCx(PgGetGC(), NULL, 1, 0);
	PgSetStrokeColor( Pg_BLUE );
	for(int i = 0; i < num_points - 1 ; i++)
			PgDrawILine( plot_point[i+1].x, plot_point[i+1].y, plot_point[i].x, plot_point[i].y );
	prev_point = plot_point[num_points - 1];
	PtClipRemove();
}

int setup( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

	mkfifo(fifo_name, S_IRUSR);
	FILE *fp = fopen(fifo_name, "r");
	float a, b, avg;
	char average[5];
	char c, buffer[1000];
	int index = 0, count = 0, two_points = 0;
	PhCursorInfo_t mouse_buf;

	prev_point.x = 0;
	prev_point.y = 0;

	while(true)
	{
		if(fscanf( fp, "%c", &c ) == EOF)
			break;
		while( c != '\n')
		{
			buffer[index] = c;
			index++;
			if(fscanf( fp, "%c", &c ) == EOF)
				return Pt_CONTINUE;
		}
		index = 0;
		count ++;

		if(count <= 9 || count == 12)
		{
			strcpy(buffer,"");
			continue;
		}
		else if(count == 10)
		{
			char field[10];
			for(int i = 0; i < col_num - 1; i++)
				memcpy(buffer, buffer + 18, sizeof(buffer) - 18);
			sscanf(buffer, "%s", field);
			PtSetResource(ABW_yaxis, Pt_ARG_TEXT_STRING, field,0);
			strcpy(buffer,"");
			continue;
		}
		else if (count == 11)
		{
			char unit[10];
			for(int i = 0; i < col_num - 1; i++)
				memcpy(buffer, buffer + 18, sizeof(buffer) - 18);
			sscanf(buffer, "%s", unit);
			PtSetResource(ABW_y_unit, Pt_ARG_TEXT_STRING, unit,0);
			strcpy(buffer,"");
			continue;
		}
		sscanf(buffer, "%f", &a);
		for(int i = 0; i < col_num - 1; i++)
			memcpy(buffer, buffer + 18, sizeof(buffer) - 18);
		if(sscanf(buffer, "%f", &b) == EOF)
		{
			printf("\n Invalid column number");
			return Pt_CONTINUE;
		}
		strcpy(buffer,"");
		points.add_point(a, b);
		two_points++;

		num_points = points.no_points();
		curr_point = points.get_points(num_points -1);

		if(points.redraw_needed() == 0)
		{
			avg = points.get_average();
			sprintf(average,"%f", avg);
			PtSetResource(ABW_average, Pt_ARG_TEXT_STRING, average,0);
			PgSetStrokeColor( Pg_BLUE );
			if(prev_point.x != 0 && prev_point.y != 0 && curr_point.x != 0 && curr_point.y != 0 && two_points != 1)
				PgDrawILine( prev_point.x, prev_point.y, curr_point.x, curr_point.y );
			prev_point = curr_point;
			PhQueryCursor( 0, &mouse_buf);
		}
		else if(points.redraw_needed() == 1)
		{
			PtDamageWidget( ABW_graph_base_raw );
			PtFlush();
			points.set_redrawn();
		}
		PtBkgdHandlerProcess();
			delay(20);
	}
	return( Pt_CONTINUE );
}


int resize( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	PhRect_t rect;
	PtCalcCanvas( ABW_base, &rect );

	Ph_area temp;
	temp.size.h = rect.lr.y - 30;
	temp.size.w = rect.lr.x - 16;
	temp.pos.x = 23;
	temp.pos.y = 16;

	points.set_axes( 23,  (temp.size.h / 2) + 16);
	points.set_dim_graph(temp.size.w , temp.size.h );
	PtSetResource( ABW_graph_base_raw, Pt_ARG_AREA, &temp, 0 );
	PtDamageWidget( ABW_graph_base_raw );
	PtFlush();
	return( Pt_CONTINUE );
}

/* Application Options string */
const char ApOptions[] =
	AB_OPTIONS ""; /* Add your options in the "" */

int ph_main( int argc, char *argv[] )
{
	/* eliminate 'unreferenced' warnings */
	argc = argc, argv = argv;
	strcpy(fifo_name, argv[1]);
	col_num = atoi(argv[2]);
	return( Pt_CONTINUE );
}


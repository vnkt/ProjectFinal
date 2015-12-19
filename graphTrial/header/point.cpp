/*
 * point.cpp
 *
 *  Created on: Feb 29, 2012
 *      Author: user
 */
#include "point.hpp"
#include <stdlib.h>

void graph::add_point(float a, float b)
{
	act_point[point_count].x = a;
	act_point[point_count].y = b;
	sum += b;
	point_count ++;

	if(point_count == 7000)
	{
		graph_reset();
		return;
	}

	if( a >= max_x )
	{
		redraw = 1;
		max_x += 5;
		return;
	}

	if( b < low_y )
	{
		redraw = 1;
		low_y = b;
		return;
	}
	else if ( b > max_y )
	{
		redraw = 1;
		max_y = b;
		return;
	}
}

graph::graph(float a, float b)
{
	point_count = 0;
	x_axis = a;
	y_axis  = b;

	height = 210;
	width = 482;

	max_x = 5;
	max_y = -100;
	low_x = 0;
	low_y = 100;

	redraw = 0;
	sum = 0;
}

graph::graph()
{
	point_count = 0;
}

graph::~graph()
{

}

void graph::scale_points()
{
	float x = width / max_x;
	if(low_y < 0 && max_y > 0)
	{
		float ly = height / ( low_y );
		float my = height / ( max_y );
		for(int i = 0; i < point_count ; i++)
		{
			cur_point[i].x = act_point[i].x * x;
			cur_point[i].x += x_axis;
			if( act_point[i].y < 0 )
			{
				cur_point[i].y = act_point[i].y * ly;
				cur_point[i].y += y_axis;
			}
			else
			{
				cur_point[i].y = act_point[i].y * my;
				cur_point[i].y = y_axis - cur_point[i].y ;
			}
		}
	}

	else if(max_y <= 0)
	{
		float ly = height / ( low_y );
		for(int i = 0; i < point_count ; i++)
		{
			cur_point[i].x = act_point[i].x * x;
			cur_point[i].x += x_axis;
			cur_point[i].y = act_point[i].y * ly;
			cur_point[i].y += y_axis;
		}
	}

	else if(low_y >= 0)
	{
		float ly = height / ( max_y );
		for(int i = 0; i < point_count ; i++)
		{
			cur_point[i].x = act_point[i].x * x;
			cur_point[i].x += x_axis;
			cur_point[i].y = act_point[i].y * ly;
			cur_point[i].y = y_axis - cur_point[i].y ;
		}
	}
}

point_det* graph::get_points()
{
	if(redraw == 1)
		fix_units();
	scale_points();
	return cur_point;
}

point_det graph::get_points(int pos)
{
	scale_points();
	return cur_point[pos];
}

int graph::no_points()
{
	return point_count;
}

float* graph::get_x_point()
{
	return x_point;
}

float* graph::get_y_point()
{
	return y_point;
}

void graph::fix_units()
{
	int i;
	float x_inter = (max_x) / 12;
	float x = width / max_x;
	for(int i = 0 ; i < 13; i++)
	{
		x_point[i] = (i) * x_inter;
		x_point_pos[i] = x_point[i] * x;
		x_point_pos[i] += x_axis;
	}

	if(low_y < 0 )
	{
		float ly = height / ( 0 - low_y );
		float y_inter = ( 0 - low_y) / 6;
		for(i = 0 ; i < 6; i++)
		{
			y_point[i + 6] = (- 1) * (i + 1 ) * y_inter;
			y_point_pos[i + 6] = (-1) * y_point[i + 6] * ly ;
			y_point_pos[i + 6] += y_axis;
		}
	}
	if(max_y > 0)
	{
		float my = height / ( max_y );
		float y_inter = (max_y) / 6;
		for(i = 0 ; i < 6; i++)
		{
			y_point[i] = (i + 1) * y_inter;
			y_point_pos[i] = y_point[i] * my;
			y_point_pos[i] = y_axis - y_point_pos[i] ;
		}
	}
}

int graph::redraw_needed()
{
	return redraw;
}

void graph::set_axes(float a, float b)
{
	x_axis = a;
	y_axis = b;
	redraw = 1;
}

void graph::set_dim_graph(int a, int b)
{
	width = a;
	height = b / 2;
	redraw = 1;
}

int graph::get_x_axis()
{
	return x_axis;
}

int graph::get_y_axis()
{
	return y_axis;
}

int graph::get_width_axis()
{
	return width;
}

float *graph::get_x_point_pos()
{
	return x_point_pos;
}

float *graph::get_y_point_pos()
{
	return y_point_pos;
}

int graph::get_height_axis()
{
	return height * 2;
}

float graph::get_average()
{
	return (sum / point_count);
}

void graph::graph_reset()
{
	point_count = 0;

	max_x = 150;
	max_y = -100;
	low_x = 0;
	low_y = 100;

	redraw = 0;
	sum = 0;

	for(int i = 0; i < 7000; i++)
	{
		cur_point[i].x = 0;
		cur_point[i].y = 0;
		act_point[i].x = 0;
		act_point[i].y = 0;
	}
}

void graph::set_redrawn()
{
	redraw = 0;
}

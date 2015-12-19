/*
 * point.hpp
 *
 *  Created on: Feb 29, 2012
 *      Author: user
 */

#ifndef POINT_HPP_
#define POINT_HPP_

struct point_det
{
	float x;
	float y;
};

class graph
{
	private:
			int point_count;
			float low_x, max_x;
			float low_y, max_y;
			float sum;

			float x_point[13];
			float y_point[12];
			float x_point_pos[13];
			float y_point_pos[12];
			float x_axis, y_axis;

			point_det act_point[7000];
			point_det cur_point[7000];

			int redraw;
			int width, height;

			void fix_units();
			void scale_points();
			void graph_reset();

	public:
			graph();
			graph(float a, float b);
			~graph();

			void add_point(float a, float b);
			point_det* get_points();
			point_det get_points(int pos);
			int no_points();
			float get_average();

			float* get_x_point();
			float* get_y_point();
			float* get_x_point_pos();
			float* get_y_point_pos();

			int redraw_needed();
			void set_axes(float a, float b);
			void set_dim_graph(int a, int b);

			int get_x_axis();
			int get_y_axis();
			int get_width_axis();
			int get_height_axis();
			void set_redrawn();
};

#endif /* POINT_HPP_ */

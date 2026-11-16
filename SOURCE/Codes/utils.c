//---------------------------------------------------------
// file:	utils.c
// author:  Chan Qi Ying, 
//          Claire Kuan Wai Cheong
// email:   q.chan@digipen.edu
//          c.cheong@digipen.edu
//
// brief:   Contain calculation to check if mouse if clicking on
//			area of rectangle/circle, calculate distance between 
//			rectangles, calculate distance between circles and
//			calculate distance between rectangle and circle. Also,
//			a function to find middle point betwwen two points.
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include <math.h>
#include "cprocessing.h"

int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y)
{
	float boxx_mini = area_center_x - area_width / 2;
	float boxx_max = area_center_x + area_width / 2;
	float boxy_mini = area_center_y - area_height / 2;
	float boxy_max = area_center_y + area_height / 2;


	if ((boxx_mini < click_x) && (click_x < boxx_max) && (boxy_mini < click_y) && (click_y < boxy_max))
	{
		return 1;
	}
	return 0;
}

int IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y)
{
	float circle_radius = diameter / 2.0f;
	float circle_distance = (float)sqrt(pow(click_x - circle_center_x, 2) + pow(click_y - circle_center_y, 2));

	if (circle_distance <= circle_radius)
	{
		return 1;
	}
	return 0;
}


int AreCirclesIntersecting(float c1_x, float c1_y, float r1, float c2_x, float c2_y, float r2)
{
	double distance = sqrt(pow(c1_x - c2_x, 2) + pow(c1_y - c2_y, 2));
	if ((float)distance <= r1 + r2) {
		return 1; 
	}
	else {
		return 0; 
	}
}

float FindMiddle(float value, float min, float max) {
	if (value < min) return min; if (value > max) return max; return value;
}

int AreRectCircleIntersecting(float circle_center_x, float circle_center_y, float diameter, float rect_center_x, float rect_center_y, float rect_width, float rect_height) {
	float radius = diameter / 2.0f;
	float closest_x = FindMiddle(circle_center_x, rect_center_x, rect_center_x + rect_width);
	float closest_y = FindMiddle(circle_center_y, rect_center_y, rect_center_y + rect_height);
	float distance_x = circle_center_x - closest_x;
	float distance_y = circle_center_y - closest_y;
	float distance = (float)sqrt(pow(distance_x, 2) + pow(distance_y, 2));
	return (distance <= radius) ? 1 : 0;
}



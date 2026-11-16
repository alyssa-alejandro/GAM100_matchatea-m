//---------------------------------------------------------
// file:	utils.h
// author:  Chan Qi Ying, 
//          Claire Kuan Wai Cheong
// email:   q.chan@digipen.edu
//          c.cheong@digipen.edu
//
// brief:   Header file for function declaration in utils.c
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once

int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);
int IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y);
int AreCirclesIntersecting(float c1_x, float c1_y, float r1, float c2_x, float c2_y, float r2);
float FindMiddle(float value, float min, float max); 
int AreRectCircleIntersecting(float circle_center_x, float circle_center_y, float diameter, float rect_center_x, float rect_center_y, float rect_width, float rect_height);

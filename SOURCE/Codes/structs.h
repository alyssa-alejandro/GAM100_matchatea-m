//---------------------------------------------------------
// file:	structs.h
// author:  Chan Qi Ying, 
//          Alyssa Nicole Cerrero Alejandro, 
//          Ng Woon Ning, 
//          Claire Kuan Wai Cheong, 
//          Low Jia En
// email:   q.chan@digipen.edu
//          alejandro.a@digipen.edu
//          woonning.ng@digipen.edu
//          c.cheong@digipen.edu
//          jiaen.low@digipen.edu
//
// brief:   Contain structs used in files and function declaration
//			for player_fns.c
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once


#define INGREDIENT_ARR_SIZE 5

typedef enum {
	sugar,
	milk,
	honey,
	condensed_milk,
	lemon,
	earl_grey,
	matcha,
	oolong,
	pearl,
	rotten,
	none
} ingredient_types;

typedef struct tea_struct { 
	char name[50]; 
	ingredient_types ingredients[INGREDIENT_ARR_SIZE];
	int ingredients_needed[INGREDIENT_ARR_SIZE];
} tea;

typedef struct cup_struct {
	float x_coor;
	float y_coor;
	float width;
	float height;

	int tea_index;			// index of the tea type
	int ingredient_count[INGREDIENT_ARR_SIZE];	// array to hold the count of ingredients
	int ingredient_count_remaining[5];
} cup;

typedef struct draw_struct {
    float x_coor;
    float y_coor;
    float width;
    float height;
} draw;

typedef struct ingredient_struct {
	ingredient_types ingredient;
	int is_moving;
	int is_rotten;
	int start_time;	
	int countdown;
	CP_Color color;
	float x_coor;
	float y_coor;
	float y_coor_land;
	float width;
	float height;
	int points_counted; 
} ingredients;

typedef struct player_struct {
	CP_Color agent_color;
	float current_coords[2];
	int is_moving_dir[4];
	float rotation;
	float min_max_xy[4];
	ingredient_types active_held_ingredient;
	CP_Color ingredient_color;
	CP_Image image;
} player;

void Render_Players(player players[]);
void Direction_Checks(player players[]);
void Key_Check(player players[]);
void Should_Move(player players[]);
void DrawString(player players[]);

void reset_cup(cup cup_objs[]); 
int put_ingredient_in_cup(ingredient_types ingredient_added, cup* cup_obj, tea tea_types[]);
void update_ingredient_count_in_cup(cup cup_objs[], tea tea_types[]);
int cup_has_been_completed(cup* cup_obj, tea tea_types[]);
void recipe_list(tea tea_types[]);

int timer_minutes(int start_time, int time_limit, int current_time);
int timer_seconds(int start_time, int time_limit, int current_time);
void P1_Hold_Throw(player player[], int num, float kiosk_h, ingredients ingredient_on_ground_arr[], int num_of_ingredients);
void P2_Hold_Throw(player player[], ingredients ingredient_on_ground_arr[], int num_of_ingredients, draw trashcan, int num_of_cups, cup cup_objs[], tea tea_types[]);

void reroll_cup_recipe(cup* cup_obj, tea* tea, int tea_idx);

void set_up_cups(cup cup_objs[], int levelnum);
int random_recipe();
void randomization_recipes(tea tea_types[], int num_recipes);


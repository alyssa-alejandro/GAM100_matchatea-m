//---------------------------------------------------------
// file:	player_fns.c
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
// brief:   Contains functions to be called from game.c
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "cprocessing.h"
#include "vars.h"
#include "structs.h"
#include "utils.h"
#include "game_scoring.h"

#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

extern game_score current_game_score;


void Render_Players(player players[]) {
        if (players[0].is_moving_dir[1]) {
            CP_Image_DrawAdvanced(players[0].image, players[0].current_coords[0], players[0].current_coords[1], player_image_size, player_image_size, 255, 90);
        }
        else if (players[0].is_moving_dir[2]) {
            CP_Image_DrawAdvanced(players[0].image, players[0].current_coords[0], players[0].current_coords[1] + 10.0f, player_image_size, player_image_size, 255, 180);
        }
        else if (players[0].is_moving_dir[3]) {
            CP_Image_DrawAdvanced(players[0].image, players[0].current_coords[0], players[0].current_coords[1] , player_image_size, player_image_size, 255, 270);
        }
        else {
            CP_Image_DrawAdvanced(players[0].image, players[0].current_coords[0], players[0].current_coords[1] - 10.0f, player_image_size, player_image_size, 255, 0);
        }

        if (players[1].is_moving_dir[1]) {
            CP_Image_DrawAdvanced(players[1].image, players[1].current_coords[0], players[1].current_coords[1], player_image_size, player_image_size, 255, 90);
        }
        else if (players[1].is_moving_dir[2]) {
            CP_Image_DrawAdvanced(players[1].image, players[1].current_coords[0], players[1].current_coords[1] + 10.0f, player_image_size, player_image_size, 255, 180);
        }
        else if (players[1].is_moving_dir[3]) {
            CP_Image_DrawAdvanced(players[1].image, players[1].current_coords[0], players[1].current_coords[1], player_image_size, player_image_size, 255, 270);
        }
        else {
            CP_Image_DrawAdvanced(players[1].image, players[1].current_coords[0], players[1].current_coords[1] - 10.0f, player_image_size, player_image_size, 255, 0);
        }
        
}

void Direction_Checks(player players[]) {
    players[0].is_moving_dir[0] = CP_Input_KeyDown(KEY_W);
    players[0].is_moving_dir[1] = CP_Input_KeyDown(KEY_A);
    players[0].is_moving_dir[2] = CP_Input_KeyDown(KEY_S);
    players[0].is_moving_dir[3] = CP_Input_KeyDown(KEY_D);

    players[1].is_moving_dir[0] = CP_Input_KeyDown(KEY_UP);
    players[1].is_moving_dir[1] = CP_Input_KeyDown(KEY_LEFT);
    players[1].is_moving_dir[2] = CP_Input_KeyDown(KEY_DOWN);
    players[1].is_moving_dir[3] = CP_Input_KeyDown(KEY_RIGHT);
}


void Key_Check(player players[]) {
    for (int i = 0; i < 2; i++) {
        player agent_obj = players[i];

        if (agent_obj.is_moving_dir[0]) {
            if (agent_obj.current_coords[1] < agent_obj.min_max_xy[1])
                agent_obj.current_coords[1] = agent_obj.min_max_xy[1] + 1;
        }
        else if (agent_obj.is_moving_dir[2]) {
            if (agent_obj.current_coords[1] > agent_obj.min_max_xy[3])
                agent_obj.current_coords[1] = agent_obj.min_max_xy[3] - 1;
        }
        if (agent_obj.is_moving_dir[1]) {
            if (agent_obj.current_coords[0] < agent_obj.min_max_xy[0])
                agent_obj.current_coords[0] = agent_obj.min_max_xy[0] + 1;
        }
        else if (agent_obj.is_moving_dir[3]) {
            if (agent_obj.current_coords[0] > agent_obj.min_max_xy[2])
                agent_obj.current_coords[0] = agent_obj.min_max_xy[2] - 1;
        }

        players[i] = agent_obj;
    }
}

void Should_Move(player players[]) {
    float expected_x_p1 = players[0].current_coords[0] + ((players[0].is_moving_dir[1]) ? -player_speed : (players[0].is_moving_dir[3]) ? player_speed : 0.0f);
    float expected_y_p1 = players[0].current_coords[1] + ((players[0].is_moving_dir[0]) ? -player_speed : (players[0].is_moving_dir[2]) ? player_speed : 0.0f);

    float expected_x_p2 = players[1].current_coords[0] + ((players[1].is_moving_dir[1]) ? -player_speed : (players[1].is_moving_dir[3]) ? player_speed : 0.0f);
    float expected_y_p2 = players[1].current_coords[1] + ((players[1].is_moving_dir[0]) ? -player_speed : (players[1].is_moving_dir[2]) ? player_speed : 0.0f);

    float vec_length = CP_Vector_Length(CP_Vector_Subtract(CP_Vector_Set(expected_x_p1, expected_y_p1), CP_Vector_Set(expected_x_p2, expected_y_p2)));

    if (vec_length <= max_string_length) {
        players[0].current_coords[0] = expected_x_p1;
        players[0].current_coords[1] = expected_y_p1;
        players[1].current_coords[0] = expected_x_p2;
        players[1].current_coords[1] = expected_y_p2;
        string_taut = 0;
    }
    else {
        string_taut = 1;
    }

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 4; j++) {
            players[i].is_moving_dir[j] = 0;
        }
    }
}

void DrawString(player players[]) {
    CP_Settings_Stroke(string_taut ? CP_Color_Create(255, 40, 20, 255) : CP_Color_Create(255, 160, 20, 255));
    CP_Graphics_DrawLine(players[0].current_coords[0], players[0].current_coords[1],
        players[1].current_coords[0], players[1].current_coords[1]);
    CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));
}

void P1_Hold_Throw(player player[], int num, float kiosk_h, ingredients ingredient_on_ground_arr[], int num_of_ingredients) { 
    // picking up an ingredient
	if ((player[0].active_held_ingredient == none) && ((int)player[0].current_coords[1] <= (int)kiosk_h)) {
		ingredient_types ingredient = player[0].active_held_ingredient;
		CP_Color color = player[0].ingredient_color;
		switch (num) {
		case 1:
			ingredient = sugar;
			color = CP_Color_Create(225, 255, 255, 255);
			break;
		case 2:
			ingredient = milk;
			color = CP_Color_Create(89, 226, 242, 255);
			break;
		case 3:
			ingredient = honey;
			color = CP_Color_Create(226, 189, 126, 255);
			break;
		case 4:
			ingredient = condensed_milk;
			color = CP_Color_Create(126, 152, 226, 255);
			break;
		case 5:
			ingredient = lemon;
			color = CP_Color_Create(243, 238, 73, 255);
			break;
		case 6:
			ingredient = earl_grey;
			color = CP_Color_Create(214, 155, 236, 255);
			break;
		case 7:
			ingredient = matcha;
			color = CP_Color_Create(174, 236, 155, 255);
			break;
		case 8:
			ingredient = oolong;
			color = CP_Color_Create(233, 214, 118, 255);
			break;
		case 9:
			ingredient = pearl;
			color = CP_Color_Create(222, 184, 135, 255);
			break;
		}
		player[0].active_held_ingredient = ingredient;
		player[0].ingredient_color = color;
	}
	// throwing an ingredient
	else if (player[0].active_held_ingredient != none) {
		for (int i = 0; i < num_of_ingredients; i++) {
            for (int i = 0; i < num_of_ingredients; i++) {
                if (ingredient_on_ground_arr[i].ingredient == none) {
                    ingredient_on_ground_arr[i].ingredient = player[0].active_held_ingredient;
                    ingredient_on_ground_arr[i].x_coor = player[0].current_coords[0];
                    ingredient_on_ground_arr[i].y_coor = player[0].current_coords[1];
                    ingredient_on_ground_arr[i].color = player[0].ingredient_color;
                    ingredient_on_ground_arr[i].points_counted = 0;  
                    player[0].active_held_ingredient = none;
                    break;
                }

            }
			
		}
	}
}


void P2_Hold_Throw(player player[], ingredients ingredient_on_ground_arr[], int num_of_ingredients,
    draw trashcan, int num_of_cups, cup cup_objs[], tea tea_types[]) {
    if (player[1].active_held_ingredient == none) {
        ingredient_types ingredient = none;
        int arr_idx = -1;   // for reset later

        for (int i = num_of_ingredients - 1; i >= 0; i--) {
            if (AreRectCircleIntersecting(player[1].current_coords[0], player[1].current_coords[1],
                player_size, ingredient_on_ground_arr[i].x_coor, ingredient_on_ground_arr[i].y_coor,
                ingredient_on_ground_arr[i].width, ingredient_on_ground_arr[i].height))
            {
                ingredient = ingredient_on_ground_arr[i].ingredient;
                player[1].ingredient_color = ingredient_on_ground_arr[i].color;
                arr_idx = i;
                break;
            }
        }

        player[1].active_held_ingredient = ingredient;

        if (arr_idx != -1) {
            ingredient_on_ground_arr[arr_idx].ingredient = none;
            ingredient_on_ground_arr[arr_idx].x_coor = 0.0f;
            ingredient_on_ground_arr[arr_idx].y_coor = 0.0f;
        }
    }
    else if (player[1].active_held_ingredient != none) {
        if (AreRectCircleIntersecting(player[1].current_coords[0], player[1].current_coords[1],
            player_size, trashcan.x_coor, trashcan.y_coor, trashcan.width, trashcan.height))
        {
            // Score points for properly disposing rotted ingredients
            if (player[1].active_held_ingredient == rotten) update_points_rot_disposed(&current_game_score);
            player[1].active_held_ingredient = none;
        }
        else {
            for (int cup_idx = 0; cup_idx < num_of_cups; cup_idx++) {
                if (AreRectCircleIntersecting(player[1].current_coords[0], player[1].current_coords[1],
                    player_size, cup_objs[cup_idx].x_coor, cup_objs[cup_idx].y_coor,
                    cup_objs[cup_idx].width, cup_objs[cup_idx].height))
                {
                    int has_put = put_ingredient_in_cup(player[1].active_held_ingredient, &cup_objs[cup_idx], tea_types);
                    if (has_put) {
                        player[1].active_held_ingredient = none;
                    }

                    update_ingredient_count_in_cup(cup_objs, tea_types);

                    // Check for cup completion and award points
                    if (cup_has_been_completed(&cup_objs[cup_idx], tea_types)) {
                        update_points_cup_complete(&current_game_score);
                        reroll_cup_recipe(&cup_objs[cup_idx], &tea_types[cup_objs[cup_idx].tea_index],
                            cup_objs[cup_idx].tea_index);
                        update_ingredient_count_in_cup(cup_objs, tea_types);
                    }
                }
            }
        }
    }
}

void reset_cup(cup cup_objs[]) {
    for (int i = 0; i < 4; i++) {
        cup_objs[i].tea_index = 0;
        for (int j = 0; j < INGREDIENT_ARR_SIZE; j++) {
            cup_objs[i].ingredient_count[j] = 0;
            cup_objs[i].ingredient_count_remaining[j] = 0;
        }
    }
}

int put_ingredient_in_cup(ingredient_types ingredient_added, cup* cup_obj, tea tea_types[]) {
    int tea_index = (*cup_obj).tea_index;
    int wrong_ingredient = 1;       // check if ingredient being put in is incorrect
    for (int i = 0; i < INGREDIENT_ARR_SIZE; i++) {
        if (ingredient_added == tea_types[tea_index].ingredients[i]) {
            if ((*cup_obj).ingredient_count[i] < tea_types[tea_index].ingredients_needed[i]) {
                wrong_ingredient = 0;
                ++(*cup_obj).ingredient_count[i];
            }
            else {
            }
        }
    }

    if (wrong_ingredient) {
        render_wrong_ingredient_msg = 5.0f;     // start render for wrong ingredient msg
        printf("----- wrong ingredient\n");
    }
    return !wrong_ingredient;
}

void update_ingredient_count_in_cup(cup cup_objs[], tea tea_types[]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < INGREDIENT_ARR_SIZE; j++) {
            cup_objs[i].ingredient_count_remaining[j] = tea_types[cup_objs[i].tea_index].ingredients_needed[j] - cup_objs[i].ingredient_count[j];
        }
    }
}


int timer_minutes(int start_time, int time_limit, int current_time) {
    time_limit -= (int)difftime(current_time, start_time);
    int minutes = time_limit / 60;
    if (minutes < 0) minutes = 0;
    return minutes;
}

int timer_seconds(int start_time, int time_limit, int current_time) {
    time_limit -= (int)difftime(current_time, start_time);
    int seconds = time_limit % 60;
    if (seconds < 0) seconds = 0;
    return seconds;
}

int cup_has_been_completed(cup* cup_obj, tea tea_types[]) {
    int tea_index = (*cup_obj).tea_index;
    int is_full = 1;

    for (int i = 0; i < INGREDIENT_ARR_SIZE; i++) {
        if ((*cup_obj).ingredient_count[i] < tea_types[tea_index].ingredients_needed[i]) {
            is_full = 0;
        }
    }
    return is_full;
}
// -- randomization the recipes 
void randomization_recipes(tea tea_types[], int num_recipes) {
	// Seed the random number generator
	srand((unsigned int)time(NULL));

	for (int i = num_recipes - 1; i > 0; i--) {
		int j = rand() % (i + 1);
        if (j <= 0) j = 1;
        // Generate a random index to swap with
		tea temp = tea_types[i];
		tea_types[i] = tea_types[j];
		tea_types[j] = temp;
	}
}

int random_recipe() {
    return rand() % (10 - 1) + 1;
}

void recipe_list(tea tea_types[]) {
    tea_types[0] = (tea){
        "None",
        {none, none, none, none, none},
        {0, 0, 0, 0, 0}
    };
    tea_types[1] = (tea){
        "Earl Grey Tea",
        {earl_grey, honey, none, none, none},
        {3, 2, 0, 0, 0}
    };
    tea_types[2] = (tea){
        "Matcha Tea",
        {matcha, honey, none, none, none},
        {4, 2, 0, 0, 0}
    };
    tea_types[3] = (tea){
        "Oolong Tea",
        {oolong, honey, none, none, none},
        {5, 1, 0, 0, 0}
    };
    tea_types[4] = (tea){
        "Lemon Tea",
        {lemon, sugar, none, none, none},
        {3, 4, 0, 0, 0}
    };
    tea_types[5] = (tea){
        "Earl Grey Lemon Tea",
        {earl_grey, lemon, honey, none, none},
        {2, 2, 3, 0, 0}
    };
    tea_types[6] = (tea){
        "Oolong Lemon Tea",
        {oolong, lemon, sugar, none, none},
        {3, 3, 2, 0, 0}
    };
    tea_types[7] = (tea){
        "Earl Grey Oolong Tea",
        {earl_grey, oolong, sugar, none, none},
        {3, 4, 3, 0, 0}
    };
    tea_types[8] = (tea){
        "Earl Grey Milk Tea",
        {earl_grey, sugar, milk, condensed_milk, pearl},
        {3, 2, 1, 1, 2}
    };
    tea_types[9] = (tea){
        "Matcha Milk Tea",
        {matcha, sugar, milk, condensed_milk, pearl},
        {3, 2, 2, 2, 2}
    };
    tea_types[10] = (tea){
        "Oolong Milk Tea",
        {oolong, sugar, milk, condensed_milk, pearl},
        {5, 1, 1, 2, 2}
    };

}


void set_up_cups(cup cup_objs[], int levelnum) {
    for (int i = 0; i < (2 + levelnum); i++) {
        cup_objs[i].tea_index = random_recipe();
    }
}


void reroll_cup_recipe(cup* cup_obj, tea* tea, int tea_idx) {
    (*cup_obj).tea_index = random_recipe();
	for (int i = 0; i < INGREDIENT_ARR_SIZE; i++) {
		(*cup_obj).ingredient_count[i] = 0;
		(*cup_obj).ingredient_count_remaining[i] = (*tea).ingredients_needed[i] - (*cup_obj).ingredient_count[i];
	}
}




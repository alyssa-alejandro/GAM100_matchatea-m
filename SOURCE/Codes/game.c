//---------------------------------------------------------
// file:	game.c
// author:  Chan Qi Ying, 
//          Alyssa Nicole Cerrero Alejandro, 
//          Ng Woon Ning, 
//          Claire Kuan Wai Cheong, 
//          Low Jia En
// email:   q.chan@digipen.edu
//          woonning.ng@digipen.edu
//          jiaen.low@digipen.edu
//          alejandro.a@digipen.edu
//          c.cheong@digipen.edu
//
// brief:   Entry point for actual game play
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#include "game_scoring.h"

#include "utils.h"
#include "mainmenu.h"
#include "cprocessing.h"
#include "player_fns.h"
#include "structs.h"
#include "page.h"
#include "screen.h"
#include "vars.h"


#define C_PI 3.1416f
#define TEA_TYPES 11 

#define MAX_INGREDIENTS 10      // maximum number of ingredients that can be handled in the game
#define INGREDIENT_HITBOX 80 

game_score current_game_score;  
int game_minutes = 0;
int game_seconds = 0;
int game_time_limit = 120;

ingredients ingredients_on_ground[MAX_INGREDIENTS];

// Player 1
float pl1_x = 800.0f;
float pl1_y = 200.0f;

// Player 2
float pl2_x = 800.0f;
float pl2_y = 500.0f;

// Counter (Where player 1 will be bounded to)
float counter_x = 0;
float counter_y = 0;
float counter_w = 1600;
float counter_h = 300;

// bounding box (where player 2 will be bounded 2) 
float boundingbox_x = 0;
float boundingbox_y = 300;
float boundingbox_w = 1600;
float boundingbox_h = 600;

float border = 30.0f;

//ingredient kiosk
float kiosk_x = 0.0f;
float kiosk_y = 0.0f;
float kiosk_w = 1600.0f / 9.0f;
float kiosk_h = 100.0f;
int num_of_ingredients = 9;
CP_Image counter_image;

CP_Image ingredient_images[10]; 

int num_of_cups = 4; 

// teacups
float recipe_displace_x = -175.0f;
float recipe_displace_y = -15.0f;

CP_Image teacup_image;
float teacup_width = 150.0f; 
float teacup_height = 125.0f;
float recipe_width = 150.0f;
float recipe_height = 175.0f;

cup cup_objs[4];

draw recipe_coords[4];

time_t start_time;
time_t current_time;
time_t paused_time;
int pausing_time;

// trash
draw trashbin;
CP_Image trashbin_image;

// timer
draw timer;
CP_Image timer_image;

// drink counter
draw drink_counter;

// Player 1 in [0], Player 2 in [1]
player player_objs[2];

tea tea_types[TEA_TYPES];

int is_game_paused = 0;

// Draw UI for score and level info
void draw_game_ui(void) {
    CP_Settings_TextSize(30.0f);
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));

    // Draw score
    char score_text[50];
    sprintf_s(score_text, sizeof(score_text), "Score: %d", current_game_score.current_points);
    CP_Font_DrawText(score_text, drink_counter.x_coor + 5.0f, drink_counter.y_coor + 30.0f);

    // Draw level
    char level_text[50];
    sprintf_s(level_text, sizeof(level_text), "Level: %d", current_game_score.current_level + 1);
    CP_Font_DrawText(level_text, drink_counter.x_coor + 5.0f, drink_counter.y_coor + 80.0f);
}

// Check level completion conditions
void check_level_completion(void) {
    if (current_game_score.level_complete) return;

    // Time's up or all cups completed 
    if ((game_minutes == 0 && game_seconds == 0)) {
        current_game_score.level_complete = 1;
        calculate_level_result(&current_game_score);
        is_game_paused = 1;
    }
}

void setup_ingredient_sprites(CP_Image ingredients[]) {
    ingredients[0] = CP_Image_Load("Assets/sugar-sprite.png");
    ingredients[1] = CP_Image_Load("Assets/milk-sprite.png");
    ingredients[2] = CP_Image_Load("Assets/honey-sprite.png");
    ingredients[3] = CP_Image_Load("Assets/condensedmilk-sprite.png");
    ingredients[4] = CP_Image_Load("Assets/lemon-sprite.png");
    ingredients[5] = CP_Image_Load("Assets/earlgrey-sprite.png");
    ingredients[6] = CP_Image_Load("Assets/matcha-sprite.png");
    ingredients[7] = CP_Image_Load("Assets/oolong-sprite.png");
    ingredients[8] = CP_Image_Load("Assets/pearl-sprite.png");
    ingredients[9] = CP_Image_Load("Assets/rotten-sprite.png");
}

void free_ingredient_sprites(CP_Image ingredients[]) {
    for (int i = 0; i < 10; i++) {
        CP_Image_Free(&ingredients[i]);
    }
}

void Game_Init(void)
{
    srand((unsigned int)time(NULL));

    CP_Settings_RectMode(CP_POSITION_CORNER);
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_BASELINE);
    CP_Settings_AntiAlias(0);
    CP_Settings_LineCapMode(CP_LINE_CAP_SQUARE);
    CP_Settings_LineJointMode(CP_LINE_JOINT_MITER);

    setup_ingredient_sprites(ingredient_images);
    timer_image = CP_Image_Load("Assets/timer-sprite.png"); 
    counter_image = CP_Image_Load("Assets/counter.png");
    teacup_image = CP_Image_Load("Assets/teacup-sprite.png");
    trashbin_image = CP_Image_Load("Assets/trashbin-sprite.png");

    // Initialize scoring system
    if (current_game_score.current_level == LEVEL_ONE) {
        init_game_score(&current_game_score);
    }


    num_of_cups = get_cups_for_level(current_game_score.current_level);

    is_game_paused = 0;

    // Initialize players
    player_objs[0] = (player){                  
        CP_Color_Create(230, 180, 230, 255),
        {pl1_x, pl1_y},
        {0, 0, 0, 0},
        0,
        {counter_x + border, kiosk_h, counter_w - border, counter_h},
        none,
        CP_Color_Create(0, 0, 0, 0),
        CP_Image_Load("Assets/bily.png")
    };

    player_objs[1] = (player){
        CP_Color_Create(180, 180, 230, 255),
        {pl2_x, pl2_y},
        {0, 0, 0, 0},
        0,
        {0.0f + border, boundingbox_y, boundingbox_w - border, counter_h + boundingbox_h - border},
        none,
        CP_Color_Create(0, 0, 0, 0),
        CP_Image_Load("Assets/lily.png")
    };

    // Initialize ingredients
    for (int i = 0; i < MAX_INGREDIENTS; i++) {
        ingredients_on_ground[i] = (ingredients){
            none,
            1, 
            0,
            0,
            0,
            CP_Color_Create(0, 0, 0, 0),
            0.0,
            0.0,
            0.0, 
            INGREDIENT_HITBOX, 
            INGREDIENT_HITBOX,
            0 
        };
    }

	// trash
	trashbin = (draw){ 20.0f, 320.0f, 100.0f, 150.0f };

	// cups L-R
	cup_objs[0] = (cup){ 600.0f, 700.0f, teacup_width, teacup_height, 0, {0,0,0,0,0}, {0,0,0,0,0} };
	cup_objs[1] = (cup){ 1000.0f, 700.0f, teacup_width, teacup_height, 0, {0,0,0,0,0}, {0,0,0,0,0} };
	cup_objs[2] = (cup){ 300.0f, 500.0f, teacup_width, teacup_height, 0, {0,0,0,0,0}, {0,0,0,0,0} };
	cup_objs[3] = (cup){ 1300.0f, 500.0f, teacup_width, teacup_height, 0, {0,0,0,0,0}, {0,0,0,0,0} };

	// recipe
	recipe_coords[0] = (draw){ cup_objs[0].x_coor + recipe_displace_x, cup_objs[0].y_coor + recipe_displace_y, recipe_width, recipe_height };
	recipe_coords[1] = (draw){ cup_objs[1].x_coor + recipe_displace_x, cup_objs[1].y_coor + recipe_displace_y, recipe_width, recipe_height };
	recipe_coords[2] = (draw){ cup_objs[2].x_coor + recipe_displace_x, cup_objs[2].y_coor + recipe_displace_y, recipe_width, recipe_height };
	recipe_coords[3] = (draw){ cup_objs[3].x_coor + recipe_displace_x, cup_objs[3].y_coor + recipe_displace_y, recipe_width, recipe_height };

	// timer
	timer = (draw){ 1350.0f, 300.0f, 240.0f, 140.0f };

	// drink counter
	drink_counter = (draw){ 1450.0f, timer.y_coor + timer.height, 120.0f, 100.0f };

    player_speed = 500 * CP_System_GetDt();

    recipe_list(tea_types);
    time(&start_time);
    pausing_time = 0;

    set_up_cups(cup_objs, (int)current_game_score.current_level);
    update_ingredient_count_in_cup(cup_objs, tea_types);
}

void Game_Update(void)
{
    time(&current_time);
    if (!is_game_paused) {
        CP_Graphics_ClearBackground(CP_Color_Create(155, 185, 193, 0));
        CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_BASELINE);
        CP_Settings_ImageFilterMode(CP_IMAGE_FILTER_NEAREST); 

        CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));

        // Draw the bounding box for player 1 
        CP_Settings_RectMode(CP_POSITION_CORNER);
        CP_Settings_Fill(CP_Color_Create(0, 169, 165, 255));
        CP_Graphics_DrawRect(counter_x, counter_y, counter_w, counter_h);

        // Drawing the bounding box for player 2
        CP_Settings_Fill(CP_Color_Create(160, 226, 164, 255));
        CP_Graphics_DrawRect(boundingbox_x, boundingbox_y, boundingbox_w, boundingbox_h);

        // trash bin print
        CP_Image_Draw(trashbin_image, trashbin.x_coor + trashbin.width / 2, trashbin.y_coor + trashbin.height / 2, trashbin.width, trashbin.height, 255);

		// Drawing the rectangle for ingredient kiosks
		float x_val = kiosk_x;
		for (int i = 0; i < num_of_ingredients; ++i) {
			switch (i) {
			case 0:
				CP_Settings_Fill(CP_Color_Create(225, 255, 255, 255));
				break;
			case 1:
				CP_Settings_Fill(CP_Color_Create(89, 226, 242, 255));
				break;
			case 2:
				CP_Settings_Fill(CP_Color_Create(226, 189, 126, 255));
				break;
			case 3:
				CP_Settings_Fill(CP_Color_Create(126, 152, 226, 255));
				break;
			case 4: 
				CP_Settings_Fill(CP_Color_Create(235, 219, 77, 255));
				break;
			case 5:
				CP_Settings_Fill(CP_Color_Create(214, 155, 236, 255));
				break;
			case 6:
				CP_Settings_Fill(CP_Color_Create(174, 236, 155, 255));
				break;
			case 7:
				CP_Settings_Fill(CP_Color_Create(233, 214, 118, 255));
				break;
			case 8:
				CP_Settings_Fill(CP_Color_Create(222, 184, 135, 255));
				break;
			}
			CP_Graphics_DrawRect(x_val, kiosk_y, kiosk_w, kiosk_h);
            CP_Image_Draw(counter_image, x_val + kiosk_w / 2, kiosk_y + kiosk_h / 2, kiosk_w, kiosk_h, 255);
            CP_Image_Draw(ingredient_images[i], x_val + kiosk_w - 30.0f, kiosk_y + 30.0f, 60.0f, 60.0f, 255);

			x_val += kiosk_w;
		}
		
        // cup print
        for (int i = 0; i < num_of_cups; ++i) {
            CP_Settings_Fill(CP_Color_Create(145, 246, 244, 255));
            CP_Image_Draw(teacup_image, cup_objs[i].x_coor + cup_objs[i].width / 2, cup_objs[i].y_coor + cup_objs[i].height / 2, cup_objs[i].width, cup_objs[i].height, 255);
            CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
            CP_Graphics_DrawRect(recipe_coords[i].x_coor, recipe_coords[i].y_coor, recipe_coords[i].width, recipe_coords[i].height);
        }

        // timer print
        CP_Settings_Fill(CP_Color_Create(235, 160, 160, 255));
        CP_Image_Draw(timer_image, timer.x_coor + timer.width / 2, timer.y_coor + timer.height / 2, timer.width, timer.height, 255);

        // drink counter/score print
        CP_Settings_Fill(CP_Color_Create(159, 169, 169, 255));
        CP_Graphics_DrawRect(drink_counter.x_coor, drink_counter.y_coor, drink_counter.width, drink_counter.height);

        // ingredient text
        CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
        text_size = 30.0f;
        CP_Settings_TextSize(text_size);
        float render_ht = kiosk_y + 92.0f;
        CP_Font_DrawText("SUGAR", 0 * kiosk_w + 5.0f, render_ht);
        CP_Font_DrawText("MILK", 1 * kiosk_w + 5.0f, render_ht);
        CP_Font_DrawText("HONEY", 2 * kiosk_w + 5.0f, render_ht);
        CP_Font_DrawText("C. MILK", 3 * kiosk_w + 5.0f, render_ht);
        CP_Font_DrawText("LEMON", 4 * kiosk_w + 5.0f, render_ht);
        CP_Font_DrawText("EARL GREY", 5 * kiosk_w + 5.0f, render_ht);
        CP_Font_DrawText("MATCHA", 6 * kiosk_w + 5.0f, render_ht);
        CP_Font_DrawText("OOLONG", 7 * kiosk_w + 5.0f, render_ht);
        CP_Font_DrawText("PEARL", 8 * kiosk_w + 5.0f, render_ht);

        // Render ingredients on ground
        for (int i = 0; i < MAX_INGREDIENTS; i++) {
            if (ingredients_on_ground[i].ingredient != none) {
                if (!ingredients_on_ground[i].y_coor_land) {
                    ingredients_on_ground[i].y_coor_land = player_objs[0].current_coords[1] + 200.0f; 
                }
                else if (ingredients_on_ground[i].y_coor_land > ingredients_on_ground[i].y_coor) {
                    ingredients_on_ground[i].y_coor += 400.0f * CP_System_GetDt();
                }
                else {
                    ingredients_on_ground[i].y_coor_land = ingredients_on_ground[i].y_coor;
                    ingredients_on_ground[i].is_moving = 0;
                }

                CP_Settings_Fill(ingredients_on_ground[i].color);
                if (!ingredients_on_ground[i].is_rotten) {
                    CP_Image_Draw(ingredient_images[ingredients_on_ground[i].ingredient], ingredients_on_ground[i].x_coor + 40.0f, ingredients_on_ground[i].y_coor + 40.0f, 80.0f, 80.0f, 255);
                }
                else {
                    CP_Image_Draw(ingredient_images[9], ingredients_on_ground[i].x_coor + 40.0f, ingredients_on_ground[i].y_coor + 40.0f, 80.0f, 80.0f, 255);
                }
            }
        }



		// ---------------------------- PLAYER KEY PORTION ----------------------------
        // Player 1: Pick ingredients from kiosk, throw to player 2
        if (CP_Input_KeyTriggered(KEY_Q)) {
			float num = (player_objs[0].current_coords[0]) / kiosk_w + 1;
			P1_Hold_Throw(player_objs, (int)num,(kiosk_h +player_size/2.0f), ingredients_on_ground, MAX_INGREDIENTS);
		}
        
        // Player 2: Pick ingredients from floor, throw into cups/trash
		if (CP_Input_KeyTriggered(KEY_SLASH)) {
			P2_Hold_Throw(player_objs, ingredients_on_ground, MAX_INGREDIENTS, trashbin, num_of_cups, cup_objs, tea_types);
		}
		// ----------------------------------------------------------------------------

        // Timer logic
        CP_Settings_TextSize(50.0f);
        int game_time_limit = 120;
        char game_time[10], ingredient_time[5];
        time(&paused_time);
        if (pausing_time != 0) {
			start_time += pausing_time;
			for (int i = 0; i < MAX_INGREDIENTS; ++i) {
				if (ingredients_on_ground[i].ingredient != none) {
					ingredients_on_ground[i].start_time += pausing_time;
				}
			}
			pausing_time = 0;
		}
        game_minutes = timer_minutes((int)start_time, game_time_limit, (int)current_time);
        game_seconds = timer_seconds((int)start_time, game_time_limit, (int)current_time);
        sprintf_s(game_time, sizeof(game_time), "%02d:%02d", game_minutes, game_seconds); 
       
        CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
        CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
        CP_Font_DrawText(game_time, timer.x_coor + (timer.width / 2.0f),
            timer.y_coor + (timer.height / 2.0f));

        // Ingredient rotting logic
        for (int i = 0; i < MAX_INGREDIENTS; ++i) {
            int ingredient_time_limit = 10;
            if (ingredients_on_ground[i].ingredient != none ) {
                if (!ingredients_on_ground[i].start_time && !ingredients_on_ground[i].is_rotten && ingredients_on_ground[i].is_moving == 0) {
                    ingredients_on_ground[i].start_time = (int)current_time;
                    ingredients_on_ground[i].countdown = ingredient_time_limit;
                }
                else if (ingredients_on_ground[i].start_time && !ingredients_on_ground[i].is_rotten) {
                    if (ingredients_on_ground[i].countdown) {
                        ingredients_on_ground[i].countdown = timer_seconds(ingredients_on_ground[i].start_time,
                            ingredient_time_limit, (int)current_time);
                    }
                    else {
                        ingredients_on_ground[i].is_rotten = 1;
                        ingredients_on_ground[i].ingredient = rotten; 
                        if (!ingredients_on_ground[i].points_counted) {
                            update_points_rot_ground(&current_game_score);
                            ingredients_on_ground[i].points_counted = 1;
                        }
                    }
                }
                else if (!ingredients_on_ground[i].countdown && ingredients_on_ground[i].is_rotten) {
                    ingredients_on_ground[i].color = CP_Color_Create(109, 127, 2, 255);
                }
            }
            else {
                // reset elements 
                ingredients_on_ground[i].start_time = 0;
                ingredients_on_ground[i].countdown = 0;
                ingredients_on_ground[i].is_rotten = 0;
                ingredients_on_ground[i].y_coor_land = 0.0f;
                ingredients_on_ground[i].is_moving = 1;
                ingredients_on_ground[i].points_counted = 0; 
            }

            // countdown not shown during ingredient displacement
            if (ingredients_on_ground[i].ingredient != none && ingredients_on_ground[i].is_moving == 0) {
                sprintf_s(ingredient_time, sizeof(ingredient_time), "%1d",
                    ingredients_on_ground[i].countdown);

                CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
                CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
                CP_Font_DrawText(ingredient_time, ingredients_on_ground[i].x_coor +
                    (ingredients_on_ground[i].width / 2), ingredients_on_ground[i].y_coor -
                    (ingredients_on_ground[i].height / 2));
            }
        }
        CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_BASELINE);

        // Recipe text display -----------------------------------------------------------------------------------------------------------------
        text_size = 18.0f;
        CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
        CP_Settings_TextSize(text_size);

        for (int teacup = 0; teacup < num_of_cups; teacup++) {
            // Display the tea type name
            CP_Font_DrawText(tea_types[cup_objs[teacup].tea_index].name,
                recipe_coords[teacup].x_coor + 5.0f, recipe_coords[teacup].y_coor + 25.0f);

            for (int j = 0; j < INGREDIENT_ARR_SIZE; j++) {
                char tea_ingred_num[20] = { 0 };
                char ics[20] = { 0 };

                sprintf_s(tea_ingred_num, 20, "     x %i", tea_types[cup_objs[teacup].tea_index].ingredients_needed[j]); 
                sprintf_s(ics, 20, "%i", cup_objs[teacup].ingredient_count[j]);

                float x_position = recipe_coords[teacup].x_coor + 20.0f;
                float y_position = recipe_coords[teacup].y_coor + 55.0f + j * 25.0f;

                // check if the ingredient is needed and draw the image
                if (tea_types[cup_objs[teacup].tea_index].ingredients[j] != none) { 
                    CP_Image_Draw(ingredient_images[tea_types[cup_objs[teacup].tea_index].ingredients[j]],
                        x_position,
                        y_position - 10.0f, 
                        25.0f, 25.0f, 255); 

                    // number of ingredients needed
                    CP_Font_DrawText(tea_ingred_num, x_position, y_position);
                    // current ingredient count
                    CP_Font_DrawText(ics, x_position + 100.f, y_position); 
                }

                
            }
        }

        // Player movement and mechanics
        Direction_Checks(player_objs);
        Key_Check(player_objs);
        Render_Players(player_objs);
        Should_Move(player_objs);
        DrawString(player_objs);

        if (player_objs[0].active_held_ingredient != none) {
            CP_Settings_Fill(player_objs[0].ingredient_color); 
            CP_Image_Draw(ingredient_images[player_objs[0].active_held_ingredient], player_objs[0].current_coords[0] + 40.0f, player_objs[0].current_coords[1] + 40.0f, 80.0f, 80.0f, 255);
        }
        if (player_objs[1].active_held_ingredient != none) {
            CP_Settings_Fill(player_objs[1].ingredient_color); 
            CP_Image_Draw(ingredient_images[player_objs[1].active_held_ingredient], player_objs[1].current_coords[0] + 40.0f, player_objs[1].current_coords[1] + 40.0f, 80.0f, 80.0f, 255);
        }

        // Wrong ingredient message
        if (render_wrong_ingredient_msg > 0.0f) {
            text_size = 50.0f;
            CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
            CP_Settings_TextSize(text_size);
            CP_Font_DrawText("Wrong Ingredient!", 600.0f, 600.0f);
            render_wrong_ingredient_msg -= 1.0f * CP_System_GetDt();
        }

        draw_game_ui();         //  Draw score and level UI
        check_level_completion();       // Check level completion
    }
    else {
        if (current_game_score.level_complete) {
            CP_Settings_RectMode(CP_POSITION_CENTER);
            CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_BASELINE);
            render_wrong_ingredient_msg = 0.0f;
            if (current_game_score.can_proceed) {
                if (!current_game_score.complete_entire_game) {
                    Win_Level(&current_game_score);  // win page for level 1/2
                }
                else {
                    Win(&current_game_score);   // win page for level 3
                }
            }
            else {
                Lose(&current_game_score);      // lose page
            }
        } 
        else {
                      
            pausing_time = (int)difftime(current_time, paused_time);

            CP_Settings_RectMode(CP_POSITION_CORNER);
            CP_Settings_Fill(CP_Color_Create(148, 168, 154, 255));
            CP_Graphics_DrawRect(0, 0, 1600, 900); 

            // Draw border rectangle
            CP_Settings_Fill(CP_Color_Create(160, 210, 164, 255));
            CP_Graphics_DrawRect(102, 68, 1396, 764);

            // "Paused" text
            CP_Settings_TextSize(150.0f);
            CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
            CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
            CP_Font_DrawText("Paused", 800, 200);  

            // Instruction text
            CP_Settings_TextSize(50.0f);
            CP_Font_DrawText("Press P to resume game", 800, 300);

            CP_Settings_Fill(CP_Color_Create(214, 212, 178, 255));
            CP_Graphics_DrawRect(600, 350, 400, 100);   // Restart button
            CP_Graphics_DrawRect(600, 475, 400, 100);   // Rules button
            CP_Graphics_DrawRect(600, 600, 400, 100);   // Exit button

            // Button text
            CP_Settings_TextSize(40.0f);
            CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
            CP_Font_DrawText("Restart Level", 800, 400);  
            CP_Font_DrawText("How to Play", 800, 525);    
            CP_Font_DrawText("Exit to Main Menu", 800, 650);     

            if (currentScreen == SCREEN_GAME) {
                CP_Settings_RectMode(CP_POSITION_CENTER);
                CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_BASELINE);
                Introduce();
            }

            float mousexaxis = CP_Input_GetMouseX();
            float mouseyaxis = CP_Input_GetMouseY();

            if (CP_Input_MouseClicked())
            {
                if (IsAreaClicked(600.0f + 400.0f / 2, 350.0f + 100.0f / 2, 400.0f, 100.0f, mousexaxis, mouseyaxis))  
                {
                    reset_game_score_at_level(&current_game_score);
                    Game_Init();
                }
                if (IsAreaClicked(600.0f + 400.0f / 2, 475.0f + 100.0f / 2, 400.0f, 100.0f, mousexaxis, mouseyaxis))    
                {
                    currentScreen = SCREEN_GAME;
                }
                if (IsAreaClicked(600.0f + 400.0f / 2, 600.0f + 100.0f / 2, 400.0f, 100.0f, mousexaxis, mouseyaxis))   
                {
                    init_game_score(&current_game_score);
                    CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit); 
                }
            }
        }
    }

    // Pause menu
    if (!current_game_score.level_complete) {
        if (CP_Input_KeyTriggered(KEY_P)) {
            is_game_paused = !is_game_paused;
            currentScreen = SCREEN_MAIN_MENU;
        }
    }

   // debug statements to test level changing
   if (CP_Input_KeyDown(KEY_LEFT_CONTROL)) {  // Only trigger when CTRL is held
       if (CP_Input_KeyTriggered(KEY_F1)) {  // CTRL + F1 = Win current level
           if (!current_game_score.complete_entire_game) {
               // Force win current level
               current_game_score.current_points = 100;  // Set a high score
               current_game_score.level_complete = 1;
               calculate_level_result(&current_game_score);
               current_game_score.can_proceed = 1;  // Ensure we can proceed
               is_game_paused = 1;
           }
       }
       if (CP_Input_KeyTriggered(KEY_F2)) {  // CTRL + F2 = Lose current level
           // Force lose current level
           current_game_score.current_points = 0;  // Set a low score
           current_game_score.level_complete = 1;
           calculate_level_result(&current_game_score); 
           current_game_score.can_proceed = 0;  // Ensure we cannot proceed
           is_game_paused = 1;
       }
       if (CP_Input_KeyTriggered(KEY_F3)) {  // CTRL + F3 = Win entire game
           // Force win entire game
           current_game_score.current_points = 100;
           current_game_score.level_complete = 1;
           current_game_score.complete_entire_game = 1;
           calculate_level_result(&current_game_score);
           current_game_score.can_proceed = 1;
           is_game_paused = 1;
       }

       // Print debug info when any of these keys are pressed with CTRL
       //if (CP_Input_KeyTriggered(KEY_F1) || CP_Input_KeyTriggered(KEY_F2) || CP_Input_KeyTriggered(KEY_F3)) {
       //    printf("Debug Info:\n");
       //    printf("Level Complete: %d\n", current_game_score.level_complete);
       //    printf("Can Proceed: %d\n", current_game_score.can_proceed);
       //    printf("Complete Game: %d\n", current_game_score.complete_entire_game);
       //    printf("Current Points: %d\n", current_game_score.current_points);
       //    printf("Current Level: %d\n", current_game_score.current_level);
       //}
   }

   
}

void Game_Exit(void)
{
    free_ingredient_sprites(ingredient_images);
    CP_Image_Free(&timer_image);
    CP_Image_Free(&counter_image);
    CP_Image_Free(&teacup_image);
    CP_Image_Free(&trashbin_image);
    CP_Image_Free(&player_objs[0].image);
    CP_Image_Free(&player_objs[1].image);
}






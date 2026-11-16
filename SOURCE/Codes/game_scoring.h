//---------------------------------------------------------
// file:	game_scoring.h
// author:  Ng Woon Ning, 
//          Low Jia En
// email:   woonning.ng@digipen.edu
//          jiaen.low@digipen.edu
//
// brief:   Contains function declarations for function defined in 
//          game_scoring.c which are to calculate game score
//          and number of stars based on number of completed cups,
//          rotten ingredients and rotten ingredients thrown.
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once

#include <stdio.h>
#include "cprocessing.h"
#include "structs.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef enum level_no {
        LEVEL_ONE = 0,
        LEVEL_TWO,
        LEVEL_THREE
    } level_no;

    typedef struct game_score {
        int cups_completed;
        int rot_on_ground;
        int rot_thrown_away;
        int current_points;
        int stars;
        level_no current_level;
        int can_proceed;
        int level_complete;
        int complete_entire_game;
    } game_score;

    void init_game_score(game_score* score);
    void reset_game_score_at_level(game_score* score);
    void update_points_cup_complete(game_score* score);
    void update_points_rot_ground(game_score* score);
    void update_points_rot_disposed(game_score* score);
    void calculate_level_result(game_score* score);
    int get_cups_for_level(level_no level);
    void proceed_to_next_level(game_score* score);

    extern game_score current_game_score;

#ifdef __cplusplus
}
#endif
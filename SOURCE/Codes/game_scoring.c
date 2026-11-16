//---------------------------------------------------------
// file:	game_scoring.c
// author:  Ng Woon Ning, 
//          Low Jia En
// email:   woonning.ng@digipen.edu
//          jiaen.low@digipen.edu
//
// brief:   Contains function definition to calculate game score
//          and number of stars based on number of completed cups,
//          rotten ingredients and rotten ingredients thrown.
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "game_scoring.h"
#include <stdio.h>

game_score current_game_score;

void init_game_score(game_score* score) {
    score->cups_completed = 0;
    score->rot_on_ground = 0;
    score->rot_thrown_away = 0;
    score->current_points = 0;
    score->stars = 0;
    score->current_level = LEVEL_ONE;
    score->can_proceed = 0;
    score->level_complete = 0;
    score->complete_entire_game = 0;
}

void reset_game_score_at_level(game_score* score) {
    score->cups_completed = 0;
    score->rot_on_ground = 0;
    score->rot_thrown_away = 0;
    score->current_points = 0;
    score->stars = 0;
    score->can_proceed = 0;
    score->level_complete = 0;
    score->complete_entire_game = 0;
}

void update_points_cup_complete(game_score* score) {
    score->cups_completed++;
    score->current_points += 10;   // +10 points per completed cup
}

void update_points_rot_ground(game_score* score) {
    score->rot_on_ground++;
    score->current_points -= 3;     // -3 points per rotted ingredient on ground
}

void update_points_rot_disposed(game_score* score) {
    score->rot_thrown_away++;
    score->current_points += 2;     // +2 points for properly disposing rotted ingredient
}

void calculate_level_result(game_score* score) {
    int total_points = score->current_points;

    // Determine stars based on points
    if (total_points < 20) {
        score->stars = 0;
        score->can_proceed = 0;
    }
    else if (total_points < 30) {
        score->stars = 1;
        score->can_proceed = 1;
    }
    else if (total_points < 40) {
        score->stars = 2;
        score->can_proceed = 1;
    }
    else {
        score->stars = 3;
        score->can_proceed = 1;
    }

    if (score->current_level == LEVEL_THREE && score->can_proceed == 1) {
        score->complete_entire_game = 1;
    }

}

int get_cups_for_level(level_no level) {
    switch (level) {
    case LEVEL_ONE:
        return 2;
    case LEVEL_TWO:
        return 3;
    case LEVEL_THREE:
        return 4;
    default:
        return 2;
    }
}

void proceed_to_next_level(game_score* score) {
    if (!score->can_proceed) return;

    switch (score->current_level) {
    case LEVEL_ONE:
        score->current_level = LEVEL_TWO;
        break;
    case LEVEL_TWO:
        score->current_level = LEVEL_THREE;
        break;
    case LEVEL_THREE:
        break;
    }

    // Reset score for new level
    score->cups_completed = 0;
    score->rot_on_ground = 0;
    score->rot_thrown_away = 0;
    score->current_points = 0;
    score->stars = 0;
    score->can_proceed = 0;
    score->level_complete = 0;
}
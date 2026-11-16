//---------------------------------------------------------
// file:	credits.c
// author:  Low Jia En
// email:   jiaen.low@digipen.edu
//
// brief:   Contains function for opening and closing credits.
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "credits.h"
#include "mainmenu.h"
#include <stdlib.h> 
#include <stdio.h>

// Credits system state
typedef struct {
    CP_Image* screens;
    int num_screens;
    int current_screen;
    float timer;
    float screen_duration;
    float fade_alpha;
    int is_fading_out;
} CreditsSystem;

// Separate systems for intro and end credits
static CreditsSystem intro_credits = { 0 };
static CreditsSystem end_credits = { 0 };

void Intro_Credits_Init(void) {
    intro_credits.num_screens = 1 ;
    intro_credits.screens = (CP_Image*)malloc(sizeof(CP_Image) * intro_credits.num_screens);

    intro_credits.screens[0] = CP_Image_Load("Assets/digipen_logo.jpg");

    intro_credits.current_screen = 0;
    intro_credits.timer = 0.0f;
    intro_credits.screen_duration = 3.0f;
    intro_credits.fade_alpha = 0.0f;
    intro_credits.is_fading_out = 0;
}

void End_Credits_Init(void) {
    printf("in end credits!\n");
    end_credits.num_screens = 3 ;
    end_credits.screens = (CP_Image*)malloc(sizeof(CP_Image) * end_credits.num_screens);

    char filename[100];
    for (int i = 0; i < end_credits.num_screens; i++) {
        sprintf_s(filename, sizeof(filename), "Assets/end_credit_%d.jpg", i + 1);
        end_credits.screens[i] = CP_Image_Load(filename);
    }

    end_credits.current_screen = 0;
    end_credits.timer = 0.0f;
    end_credits.screen_duration = 4.0f;
    end_credits.fade_alpha = 0.0f;
    end_credits.is_fading_out = 0;
}

void Intro_Credits_Update(void) {
    CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));

    // Calculate alpha for fade in and out
    float display_alpha;
    if (intro_credits.is_fading_out) {
        display_alpha = 1.0f - intro_credits.fade_alpha;        // Fade out
    }
    else if (intro_credits.timer < 1.0f) {
        display_alpha = intro_credits.timer;        // Fade in
    }
    else {
        display_alpha = 1.0f;           // Fully visible
    }

    // Draw current screen with fade
    CP_Image_Draw(intro_credits.screens[intro_credits.current_screen],
        (float)CP_System_GetWindowWidth() / 2.0f,
        (float)CP_System_GetWindowHeight() / 2.0f,
        (float)CP_System_GetWindowWidth(),
        (float)CP_System_GetWindowHeight(),
        (int)(255 * display_alpha));

    intro_credits.timer += CP_System_GetDt();

    // Handle fading logic
    if (intro_credits.is_fading_out) {
        intro_credits.fade_alpha += CP_System_GetDt() * 2.0f;  
        if (intro_credits.fade_alpha >= 1.0f) {
            intro_credits.fade_alpha = 0.0f;
            intro_credits.is_fading_out = 0;
            intro_credits.current_screen++;
            intro_credits.timer = 0.0f;          // Reset timer for next fade in

            if (intro_credits.current_screen >= intro_credits.num_screens) {
                CP_Engine_SetNextGameStateForced(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
            }
        }
    }
    else if (intro_credits.timer >= intro_credits.screen_duration) {
        intro_credits.is_fading_out = 1;
    }

    // Skip all credits with SPACE/ESC KEY
    if (CP_Input_KeyTriggered(KEY_SPACE) || CP_Input_KeyTriggered(KEY_ESCAPE) ||
        CP_Input_KeyTriggered(KEY_ENTER)) {
        CP_Engine_SetNextGameStateForced(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
    }
}

void End_Credits_Update(void) {
    // Fading credits
    CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));

    // Calculate alpha for fade in and out
    float display_alpha;
    if (end_credits.is_fading_out) {
        display_alpha = 1.0f - end_credits.fade_alpha;  // Fade out
    }
    else if (end_credits.timer < 1.0f) {
        display_alpha = end_credits.timer;          // Fade in
    }
    else {
        display_alpha = 1.0f;           // Fully visible
    }

    CP_Image_Draw(end_credits.screens[end_credits.current_screen],
        (float)CP_System_GetWindowWidth() / 2.0f,
        (float)CP_System_GetWindowHeight() / 2.0f,
        (float)CP_System_GetWindowWidth(),
        (float)CP_System_GetWindowHeight(),
        (int)(255 * display_alpha));

    end_credits.timer += CP_System_GetDt();

    // Handle fading logic
    if (end_credits.is_fading_out) {
        end_credits.fade_alpha += CP_System_GetDt() * 2.0f;  
        if (end_credits.fade_alpha >= 1.0f) {
            end_credits.fade_alpha = 0.0f;
            end_credits.is_fading_out = 0;
            end_credits.current_screen++;
            end_credits.timer = 0.0f;  

            if (end_credits.current_screen >= end_credits.num_screens) {
                CP_Engine_SetNextGameStateForced(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
            }
        }
    }
    else if (end_credits.timer >= end_credits.screen_duration) {
        end_credits.is_fading_out = 1;
    }

    // Skip all credits with SPACE/ESC KEY
    if (CP_Input_KeyTriggered(KEY_SPACE) || CP_Input_KeyTriggered(KEY_ESCAPE) ||
        CP_Input_KeyTriggered(KEY_ENTER)) {
        CP_Engine_SetNextGameStateForced(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
    }


}

void Intro_Credits_Exit(void) {
    for (int i = 0; i < intro_credits.num_screens; i++) {
        CP_Image_Free(&intro_credits.screens[i]);
    }
    free(intro_credits.screens);
}

void End_Credits_Exit(void) {
    for (int i = 0; i < end_credits.num_screens; i++) {
        CP_Image_Free(&end_credits.screens[i]);
    }
    free(end_credits.screens);
}
//---------------------------------------------------------
// file:	page.c	
// author:  Claire Kuan Wai Cheong
// email:   c.cheong@digipen.edu
// 
// brief:	Contains functions to show win or lose page.
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include <stdio.h>
#include "cprocessing.h"
#include "mainmenu.h"
#include "game.h"
#include "utils.h"
#include "screen.h"
#include "credits.h"

#include "game_scoring.h"

void stars(game_score* score) {
    CP_Settings_Fill(CP_Color_Create(207, 235, 223, 255));
    float starCentersX[3] = { 600.0f, 800.0f, 1000.0f };    // Centers with 150 pixels between each star
    float topY = 350.0f;  

    for (int i = 0; i < (*score).stars; i++) {
        CP_Graphics_BeginShape();

        float x = starCentersX[i];
        float y = topY + 50.0f;
        CP_Graphics_AddVertex(x, y - 20.0f);
        CP_Graphics_AddVertex(x + 25.0f, y + 35.0f);
        CP_Graphics_AddVertex(x + 75.0f, y + 35.0f);
        CP_Graphics_AddVertex(x + 35.0f, y + 75.0f);
        CP_Graphics_AddVertex(x + 50.0f, y + 125.0f);
        CP_Graphics_AddVertex(x, y + 90.0f);
        CP_Graphics_AddVertex(x - 50.0f, y + 125.0f);
        CP_Graphics_AddVertex(x - 35.0f, y + 75.0f);
        CP_Graphics_AddVertex(x - 75.0f, y + 35.0f);
        CP_Graphics_AddVertex(x - 25.0f, y + 35.0f);

        CP_Graphics_EndShape();
    }
}

void Win_Level(game_score* score) {
    CP_Graphics_ClearBackground(CP_Color_Create(148, 168, 154, 0));

    CP_Settings_Fill(CP_Color_Create(158, 173, 200, 100));
    CP_Graphics_DrawRect(1.0f, 1.0f, 16000.0f, 100000.0f);
    stars(score);

    // Next Button
    CP_Settings_Fill(CP_Color_Create(160, 226, 164, 255)); 
    CP_Graphics_DrawRect(800.0f, 650.0f, 250.0f, 100.0f);

    float sizetext = 150.0f;
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
    CP_Settings_TextSize(sizetext);
    CP_Font_DrawText("YOU WIN", 560, 350);
    sizetext = 50.0f;
    CP_Settings_TextSize(sizetext);
    CP_Font_DrawText("Next Level", 700, 660);

    float mousex = CP_Input_GetMouseX();
    float mousey = CP_Input_GetMouseY();
    if (CP_Input_MouseClicked()) {
        if (IsAreaClicked(800.0f, 650.0f, 250.0f, 100.0f, mousex, mousey)) {
            proceed_to_next_level(&current_game_score);
            Game_Init();
        }
    }
}

void Win(game_score* score) {
    CP_Graphics_ClearBackground(CP_Color_Create(148, 168, 154, 0));

    CP_Settings_Fill(CP_Color_Create(158, 173, 200, 100));
    CP_Graphics_DrawRect(1.0f, 1.0f, 16000.0f, 100000.0f);
    stars(score);

    CP_Settings_Fill(CP_Color_Create(160, 226, 164, 255)); 
    CP_Graphics_DrawRect(800.0f, 600.0f, 450.0f, 100.0f);

    float sizetext = 150.0f;
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
    CP_Settings_TextSize(sizetext);
    CP_Font_DrawText("YOU WIN", 560, 350);

    sizetext = 50.0f;
    CP_Settings_TextSize(sizetext);
    CP_Font_DrawText("Return to Main Menu", 600, 610);

    float mousex = CP_Input_GetMouseX();
    float mousey = CP_Input_GetMouseY();
    if (CP_Input_MouseClicked()) {
        if (IsAreaClicked(800.0f, 600.0f, 450.0f, 100.0f, mousex, mousey)) {
            init_game_score(&current_game_score);
            currentScreen = SCREEN_MAIN_MENU;
            CP_Engine_SetNextGameState(End_Credits_Init, End_Credits_Update, End_Credits_Exit);
        }
    }
}

void Lose(game_score* score) {
    CP_Graphics_ClearBackground(CP_Color_Create(148, 168, 154, 255));

    CP_Settings_Fill(CP_Color_Create(158, 173, 200, 100));
    CP_Graphics_DrawRect(1.0f, 1.0f, 16000.0f, 100000.0f);
    stars(score);

    CP_Settings_Fill(CP_Color_Create(160, 226, 164, 255));
    CP_Graphics_DrawRect(800.0f, 600.0f, 350.0f, 100.0f);

    CP_Settings_Fill(CP_Color_Create(160, 226, 164, 255));
    CP_Graphics_DrawRect(800.0f, 750.0f, 450.0f, 100.0f);

    float sizetext = 150.0f;
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
    CP_Settings_TextSize(sizetext);
    CP_Font_DrawText("YOU LOSE", 540, 350);

    sizetext = 50.0f;
    CP_Settings_TextSize(sizetext);
    CP_Font_DrawText("Restart Level", 670, 625);
    CP_Font_DrawText("Return to Main Menu", 600, 770);

    float mousex = CP_Input_GetMouseX();
    float mousey = CP_Input_GetMouseY();
    if (CP_Input_MouseClicked()) {
        if (IsAreaClicked(800.0f, 750.0f, 350.0f, 100.0f, mousex, mousey)) {
            init_game_score(&current_game_score);
            currentScreen = SCREEN_MAIN_MENU;
            CP_Engine_SetNextGameState(End_Credits_Init, End_Credits_Update, End_Credits_Exit);
        }
        if (IsAreaClicked(800.0f, 600.0f, 250.0f, 100.0f, mousex, mousey)) {
            reset_game_score_at_level(&current_game_score);
            Game_Init();
        }
    }
}

 

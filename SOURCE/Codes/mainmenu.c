//---------------------------------------------------------
// file:	mainmenu.c	
// author:  Chan Qi Ying, 
//          Ng Woon Ning, 
//          Claire Kuan Wai Cheong
// email:   q.chan@digipen.edu
//          woonning.ng@digipen.edu
//          c.cheong@digipen.edu
// 
// brief:	mainmenu contains 3 buttons, 
//          One button is "Play".When users click within the rectangle of this button, the 
//          application will transition to the Game State.
//      
//          Center is "How to Play". When users click within the rectangle of this button,
//          a rules page will appear.
// 
//          Another button is "Exit".When users click within the rectangle of this button,
//          the application will exit.
//          
//          One function is to show contents on How To Play page.
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include <stdio.h>
#include "cprocessing.h" 
#include "utils.h"
#include "game.h"
#include "screen.h"
#include "vars.h"
#include "mainmenu.h" 
#include "credits.h"

CP_Font font;
CP_TEXT_ALIGN_HORIZONTAL horizontal = CP_TEXT_ALIGN_H_LEFT;
CP_TEXT_ALIGN_VERTICAL vertical = CP_TEXT_ALIGN_V_BASELINE;

CP_Image game_logo;

//Color for button height
float play_btn_dims[4] = { 1300.0f, 150.0f, 350.0f, 100.0f };
float intro_btn_dims[4] = { 1300.0f, 350.0f, 350.0f, 100.0f };
float credits_btn_dims[4] = { 1300.0f, 550.0f, 350.0f, 100.0f };
float exit_btn_dims[4] = { 1300.0f, 750.0f, 350.0f, 100.0f };

// Helper function to draw rectangles
void DrawingRectangle(float x, float y, float width, float height, CP_Color color) {
    CP_Settings_Fill(color);
    CP_Graphics_BeginShape();
    CP_Graphics_AddVertex(x, y);
    CP_Graphics_AddVertex(x + width, y);
    CP_Graphics_AddVertex(x + width, y + height);
    CP_Graphics_AddVertex(x, y + height);
    CP_Graphics_EndShape();
}
void InstructionsForPlayer(const char* playerName, float startX, float startY, float cellWidth, float cellHeight, CP_Color textColor) {
    CP_Settings_TextSize(30.0f);
    CP_Settings_Fill(textColor);

    CP_Font_DrawText(playerName, startX, startY - cellHeight);
    CP_Font_DrawText("KEY", startX, startY);
    CP_Font_DrawText("DIRECTION", startX + cellWidth, startY);

    const char* directions[] = { "UP", "DOWN", "LEFT", "RIGHT", "Interaction Key"};
    const char* keysP1[] = { "W", "S", "A", "D", "Q"};
    const char* keysP2[] = { "Up", "Down", "Left", "Right", "/"};

    const char** keys = (strcmp(playerName, "Player 1") == 0) ? keysP1 : keysP2;

    for (int i = 0; i < 5; i++) {
        float offsetY = startY + cellHeight * (i + 1);
        CP_Font_DrawText(directions[i], startX + cellWidth, offsetY);
        CP_Font_DrawText(keys[i], startX, offsetY);
    }
}
void InstructionsToGame(float startX, float startY, float cellHeight, CP_Color textColor) {
    CP_Settings_Fill(textColor);
    CP_Settings_TextSize(30.0f);

    const char instructions[][200] = {
        "Pause/Resume Key: \"P\"",
        "Points System:",
        "-3 points for every rotten ingredient",
        "+2 points for every rotten ingredient thrown",
        "+10 points for every cup completed",
        "Level Completion:",
        "Your goal is to complete as many cups as",
        "possible within the time limit.",
        "A total of 3 stars can be obtained.",
        "However, a minimum of one star must be",
        "obtained to proceed to the next level.",
        "That's all! Happy Gaming! :)"
    };

    for (int i = 0; i < 12; i++) {
        CP_Font_DrawText(instructions[i], startX, startY + cellHeight * i);
    }
}
void Introduce(void) {
    // frequently used colors
    CP_Color bgColor = CP_Color_Create(148, 168, 154, 255);
    CP_Color tableColor = CP_Color_Create(207, 235, 223, 255);
    CP_Color textColor = CP_Color_Create(0, 0, 0, 255);
    CP_Color playButtonColor = CP_Color_Create(214, 212, 178, 255);

    // Background
    CP_Graphics_ClearBackground(bgColor);
    CP_Settings_Fill(CP_Color_Create(148, 168, 154, 255));
    CP_Graphics_DrawRect(0.0f, 0.0f, 16000.0f, 100000.0f);

    // Table configurations
    float table1X[2] = { 100.0f, 300.0f };
    float table2X[2] = { 600.0f, 800.0f };
    float topY = 140.0f;
    float cellWidth = 200.0f, cellHeight = 60.0f;

    // Draw tables
    for (int i = 0; i < 6; i++) {
        for (int col = 0; col < 2; col++) {
            float yOffset = topY + i * cellHeight;
            // Table 1
            DrawingRectangle(table1X[col], yOffset, cellWidth, cellHeight, tableColor);
            // Table 2
            DrawingRectangle(table2X[col], yOffset, cellWidth, cellHeight, tableColor);
        }
    }

    // Draw Player Instructions
    InstructionsForPlayer("Player 1", table1X[0] + 10.0f, topY + (cellHeight / 4) * 3, cellWidth, cellHeight, textColor);
    InstructionsForPlayer("Player 2", table2X[0] + 10.0f, topY + (cellHeight / 4) * 3, cellWidth, cellHeight, textColor);

    InstructionsToGame(90.0f + cellWidth * 5, 160.0f, cellHeight, textColor);

    // Draw Return Button
    float buttonX = 750.0f, buttonY = 300.0f + cellHeight * 8 + 10.0f, buttonWidth = 550.0f, buttonHeight = 80.0f;
    CP_Settings_Fill(playButtonColor);
    CP_Settings_TextSize(50.0f);
    if (currentScreen == SCREEN_GAME) {
        buttonX = 800.0f, buttonY = 300.0f + cellHeight * 8 + 10.0f, buttonWidth = 450.0f, buttonHeight = 80.0f;
        CP_Graphics_DrawRect(buttonX, buttonY, buttonWidth, buttonHeight);
        CP_Settings_Fill(textColor);
        CP_Font_DrawText("Return to Game", buttonX - 150.0f, buttonY + 10.0f);
    }
    else if (currentScreen == SCREEN_INTRODUCTION) {
        CP_Graphics_DrawRect(buttonX, buttonY, buttonWidth, buttonHeight);
        CP_Settings_Fill(textColor);
        CP_Font_DrawText("Return to Main Menu", buttonX - 200.0f, buttonY + 10.0f);
    }
        

    // Handle button click
    float mousex = CP_Input_GetMouseX(), mousey = CP_Input_GetMouseY();
    if (CP_Input_MouseClicked() && IsAreaClicked(buttonX, buttonY, buttonWidth, buttonHeight, mousex, mousey)) {
        currentScreen = SCREEN_MAIN_MENU;
    }
}
void Main_Menu_Init(void)
{
    //Font
	font = CP_Font_Load("Assets/Exo2-Regular.ttf");
	CP_Font_Set(font);
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Settings_TextAlignment(horizontal, vertical);

	game_logo = CP_Image_Load("Assets/chai-and-chain.png");

	CP_Settings_AntiAlias(0); 
	CP_Settings_LineCapMode(CP_LINE_CAP_SQUARE);
	CP_Settings_LineJointMode(CP_LINE_JOINT_MITER);
}

void Main_Menu_Update(void)
{

	CP_Graphics_ClearBackground(CP_Color_Create(148, 168, 154, 0));
    // drawing the logo
    CP_Image_Draw(game_logo, 600.0f, 400.0f, 800.0f, 450.0f, 255);

    //Text
	CP_Settings_TextSize(text_size);
	text_size = 100.0f;

    //Color for button height
    //Play Button 
	CP_Settings_Fill(CP_Color_Create(199, 63, 55, 255));    
	CP_Graphics_DrawRect(play_btn_dims[0], play_btn_dims[1], play_btn_dims[2], play_btn_dims[3]);
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	CP_Font_DrawText("Start Game", play_btn_dims[0] - play_btn_dims[2] / 2 + 30.0f, play_btn_dims[1] - play_btn_dims[3] / 2 + 70.0f); 

    //Introduce Button
	text_size = 50.0f;
	CP_Settings_Fill(CP_Color_Create(209, 213, 222, 255));  
	CP_Graphics_DrawRect(intro_btn_dims[0], intro_btn_dims[1], intro_btn_dims[2], intro_btn_dims[3]);
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	CP_Font_DrawText("How to Play", intro_btn_dims[0] - intro_btn_dims[2] / 2 + 30.0f, intro_btn_dims[1] - intro_btn_dims[3] / 2 + 70.0f);	

    CP_Settings_Fill(CP_Color_Create(214, 212, 178, 255));
    CP_Graphics_DrawRect(credits_btn_dims[0], credits_btn_dims[1], credits_btn_dims[2], credits_btn_dims[3]);
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
    CP_Font_DrawText("Credits", credits_btn_dims[0] - credits_btn_dims[2] / 2 + 30.0f, credits_btn_dims[1] - credits_btn_dims[3] / 2 + 70.0f);

    // Exit Button
	CP_Settings_Fill(CP_Color_Create(160, 226, 164, 255));  
	CP_Graphics_DrawRect(exit_btn_dims[0], exit_btn_dims[1], exit_btn_dims[2], exit_btn_dims[3]);
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255)); 
	CP_Font_DrawText("Exit", exit_btn_dims[0] - exit_btn_dims[2] / 2 + 30.0f, exit_btn_dims[1] - exit_btn_dims[3] / 2 + 70.0f);	

	float mousex = CP_Input_GetMouseX();
	float mousey = CP_Input_GetMouseY();

	if (currentScreen == SCREEN_INTRODUCTION) {
		Introduce();
		return;
	} 

	if (CP_Input_MouseClicked())
	{
		if (IsAreaClicked(play_btn_dims[0], play_btn_dims[1], play_btn_dims[2], play_btn_dims[3], mousex, mousey))
		{
			CP_Engine_SetNextGameState(Game_Init, Game_Update, Game_Exit);
		}

		if (IsAreaClicked(intro_btn_dims[0], intro_btn_dims[1], intro_btn_dims[2], intro_btn_dims[3], mousex, mousey))
		{
			currentScreen = SCREEN_INTRODUCTION;
		}
        if (IsAreaClicked(credits_btn_dims[0], credits_btn_dims[1], credits_btn_dims[2], credits_btn_dims[3], mousex, mousey))
        {
            currentScreen = SCREEN_MAIN_MENU;
            CP_Engine_SetNextGameState(End_Credits_Init, End_Credits_Update, End_Credits_Exit);
        }
		if (IsAreaClicked(exit_btn_dims[0], exit_btn_dims[1], exit_btn_dims[2], exit_btn_dims[3], mousex, mousey))
		{
			CP_Engine_Terminate();
		}
	}
}

void Main_Menu_Exit(void)
{
	CP_Image_Free(&game_logo);
	//CP_Engine_SetNextGameStateForced(Game_Init, Game_Update, Game_Exit);
}
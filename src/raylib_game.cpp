/*******************************************************************************************
*
*   raylib 9years gamejam template
*
*   Template originally created with raylib 4.5-dev, last time updated with raylib 4.5-dev
*
*   Template licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2022 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#if defined(PLATFORM_WEB)
    #define CUSTOM_MODAL_DIALOGS            // Force custom modal dialogs usage
    #include <emscripten/emscripten.h>      // Emscripten library - LLVM to JavaScript compiler
#endif

#include <stdio.h>                          // Required for: printf()
#include <stdlib.h>                         // Required for: 
#include <string.h>                         // Required for: 

#include "normal_snake.h"

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
// Simple log system to avoid printf() calls if required
// NOTE: Avoiding those calls, also avoids const strings memory usage
#define SUPPORT_LOG_INFO
#if defined(SUPPORT_LOG_INFO)
    #define LOG(...) printf(__VA_ARGS__)
#else
    #define LOG(...)
#endif

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef enum { 
    SCREEN_LOGO = 0, 
    SCREEN_TITLE, 
    SCREEN_GAMEPLAY, 
    SCREEN_ENDING
} GameScreen;

// TODO: Define your custom data types here

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
static const int screenWidth = 256;
static const int screenHeight = 256;

static unsigned int screenScale = 1; 
static unsigned int prevScreenScale = 1;

static RenderTexture2D target = { 0 };  // Initialized at init

// TODO: Define global variables here, recommended to make them static

static Camera2D cam;

static NormalSnake normalSnake;
static SnakeState curSnakeState;

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
static void UpdateDrawFrame(void);      // Update and Draw one frame

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
#if !defined(_DEBUG)
    SetTraceLogLevel(LOG_NONE);         // Disable raylib trace log messsages
#endif

    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "raylib 9yr gamejam");
    
    // TODO: Load resources / Initialize variables at this point
    cam = Camera2D{ Vector2{ 256 / 2.0f, 256 / 2.0f }, Vector2{ 0.0f, 0.0f }, 0.0f, 1.0f };

    InitApples();

    normalSnake = NormalSnake(Vector2{ 0.0f, 0.0f });
    
    // Render texture to draw full screen, enables screen scaling
    // NOTE: If screen is scaled, mouse input should be scaled proportionally
    target = LoadRenderTexture(screenWidth, screenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);     // Set our game frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button
    {
        UpdateDrawFrame();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadRenderTexture(target);
    
    // TODO: Unload all loaded resources at this point

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//--------------------------------------------------------------------------------------------
// Module functions definition
//--------------------------------------------------------------------------------------------
// Update and draw frame
void UpdateDrawFrame(void)
{
    // Update
    //----------------------------------------------------------------------------------
    // Screen scale logic (x2)
    if (IsKeyPressed(KEY_ONE)) screenScale = 1;
    else if (IsKeyPressed(KEY_TWO)) screenScale = 2;
    else if (IsKeyPressed(KEY_THREE)) screenScale = 3;
    
    if (screenScale != prevScreenScale)
    {
        // Scale window to fit the scaled render texture
        SetWindowSize(screenWidth*screenScale, screenHeight*screenScale);
        
        // Scale mouse proportionally to keep input logic inside the 256x256 screen limits
        SetMouseScale(1.0f/(float)screenScale, 1.0f/(float)screenScale);
        
        prevScreenScale = screenScale;
    }

    // TODO: Update variables / Implement example logic at this point
    //----------------------------------------------------------------------------------
    float dt = GetFrameTime();
    curSnakeState = normalSnake.Step(dt);

    // Draw
    //----------------------------------------------------------------------------------
    // Render all screen to texture (for scaling)
    BeginTextureMode(target);
        ClearBackground(RAYWHITE);
        
        // TODO: Draw screen at 256x256

        BeginMode2D(cam);
        {
            // If being eaten, draw snake under apples
            if (curSnakeState == SnakeState::EATING)
            {
                normalSnake.Draw();
            }
            DrawApples();
            if (curSnakeState == SnakeState::OK)
            {
                normalSnake.Draw();
            }
        }
        
    EndTextureMode();
    
    BeginDrawing();
        ClearBackground(RAYWHITE);
        
        // Draw render texture to screen scaled as required
        DrawTexturePro(target.texture, Rectangle{ 0, 0, (float)target.texture.width, -(float)target.texture.height }, Rectangle{ 0, 0, (float)target.texture.width*screenScale, (float)target.texture.height*screenScale }, Vector2{ 0, 0 }, 0.0f, WHITE);

        // Draw equivalent mouse position on the target render-texture
        // DrawCircleLines(GetMouseX(), GetMouseY(), 10, MAROON);

        // TODO: Draw everything that requires to be drawn at this point:

    EndDrawing();
    //----------------------------------------------------------------------------------  
}
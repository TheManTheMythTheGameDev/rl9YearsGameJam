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
#include "grid.h"
#include "physics_object.h"
#include "snake.h"
#include "hook_snake.h"

#include "screens.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <string>

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
typedef enum {
    NORMAL_SNAKE = 0,
    SNAKE,
    HOOK_SNAKE
} GameplayState;

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

static GameScreen gameState;
static GameplayState gameplayState;

static Snake snake;
static HookSnake hookSnake;

// Title screen variables
static bool showInstructions;
static Texture2D instructionsTex;

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
static void UpdateDrawFrame(void);      // Update and Draw one frame
static bool DrawButton(Vector2 position, float width, float height, std::string text);

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
    gameState = SCREEN_LOGO;
    gameplayState = NORMAL_SNAKE;

    cam = Camera2D{ Vector2{ 0.0f, 0.0f }, Vector2{0.0f, 0.0f}, 0.0f, 1.0f};

    InitApples();

    normalSnake = NormalSnake(Vector2{ 0.0f, 0.0f });

    InitGrid();
    snake = Snake(Vector2{ 64.0f, 100.0f });
    hookSnake = HookSnake(Vector2{ 32.0f, 100.0f });

    showInstructions = false;
    instructionsTex = LoadTexture("resources/textures/instructionsScreen.png");
    
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
    UnloadTexture(instructionsTex);

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
        SetWindowSize(screenWidth * screenScale, screenHeight * screenScale);

        // Scale mouse proportionally to keep input logic inside the 256x256 screen limits
        SetMouseScale(1.0f / (float)screenScale, 1.0f / (float)screenScale);

        prevScreenScale = screenScale;
    }

    // TODO: Update variables / Implement example logic at this point
    //----------------------------------------------------------------------------------
    float dt = GetFrameTime();

    switch (gameState)
    {
    case SCREEN_LOGO:
    {
        UpdateLogoScreen();
        if (FinishLogoScreen() == 1)
        {
            gameState = SCREEN_TITLE;
        }
        break;
    }
    case SCREEN_TITLE:
    {
        break;
    }
    case SCREEN_GAMEPLAY:
    {
        switch (gameplayState)
        {
        case NORMAL_SNAKE:
        {
            cam.offset = Vector2{ 256 / 2.0f, 256 / 2.0f };
            curSnakeState = normalSnake.Step(dt);
            if (curSnakeState == ENDSCENE)
            {
                gameplayState = SNAKE;
            }
            break;
        }
        case SNAKE:
        {
            cam.offset = Vector2{ 0.0f, 0.0f };
            cam.target = Vector2Subtract(snake.GetPosition(), Vector2{ 256.0f / 2.0f, 256.0f / 2.0f });

            if (cam.target.y > 0.0f)
            {
                cam.target.y = 0.0f;
            }

            snake.Update(dt);
            break;
        }
        case HOOK_SNAKE:
        {
            cam.offset = Vector2{ 0.0f, 0.0f };
            cam.target = Vector2Subtract(hookSnake.GetPosition(), Vector2{ 256.0f / 2.0f, 256.0f / 2.0f });

            if (cam.target.y > 0.0f)
            {
                cam.target.y = 0.0f;
            }

            hookSnake.Update(cam, dt);
            break;
        }
        default:
        {
            break;
        }
        }
        break;
    }
    case SCREEN_ENDING:
    {
        break;
    }
    default:
    {
        break;
    }
    }

    // Draw
    //----------------------------------------------------------------------------------
    // Render all screen to texture (for scaling)
    BeginTextureMode(target);
    {
    ClearBackground(RAYWHITE);

    // TODO: Draw screen at 256x256

    switch (gameState)
    {
    case SCREEN_LOGO:
    {
        DrawLogoScreen();
        break;
    }
    case SCREEN_TITLE:
    {
        if (!showInstructions)
        {
            if (DrawButton(Vector2{ 256.0f / 2.0f, 256.0f / 2.0f }, 100, 30, "PLAY"))
            {
                gameState = SCREEN_GAMEPLAY;
            }
            if (DrawButton(Vector2{ 256.0f / 2.0f, 200.0f }, 100, 20, "INSTRUCTIONS"))
            {
                showInstructions = true;
            }
            // Draw logo
            DrawText("ENTIRELY NORMAL", 30, 20, 20, BLACK);
            DrawText("SNAKE GAME", 27, 50, 30, BLACK);
            // Draw snake
            DrawRectangle(20, 40, 215, 10, GREEN);
            // Round ends of snake
            DrawCircle(20, 45, 5, GREEN);
            DrawCircle(235, 45, 5, GREEN);
            // Draw eyes
            DrawRectangle(229, 42, 5, 2, BLACK);
            DrawRectangle(229, 46, 5, 2, BLACK);
        }

        if (showInstructions)
        {
            DrawTexture(instructionsTex, 0, 0, WHITE);
            DrawText("INSTRUCTIONS", 20, 20, 29, BLACK);
            DrawText("TO MOVE", 120, 90, 20, BLACK);
            DrawText("EAT THE APPLES", 17, 180, 17, BLACK);
            if (DrawButton(Vector2{ 150.0f, 220.0f }, 100, 20, "BACK"))
            {
                showInstructions = false;
            }
        }

        break;
    }
    case SCREEN_GAMEPLAY:
    {
        BeginMode2D(cam);
        {
            switch (gameplayState)
            {
            case NORMAL_SNAKE:
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
                if (curSnakeState == SnakeState::ENDSCENE)
                {

                }
                break;
            }
            case SNAKE:
            {
                DrawGrid();
                snake.Draw();
                break;
            }
            default:
            {
                DrawGrid();
                hookSnake.Draw();
                break;
            }
            }
        }
        EndMode2D();
        break;
    }
    case SCREEN_ENDING:
    {
        break;
    }
    default:
    {
        break;
    }
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

static bool DrawButton(Vector2 position, float width, float height, std::string text)
{
    return (GuiButton(Rectangle{ position.x - (width / 2.0f), position.y - (height / 2.0f), width, height }, text.c_str()));
}
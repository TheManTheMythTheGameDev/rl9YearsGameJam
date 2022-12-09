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
#include "rlgl.h"

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
static const int screenWidth = 500;
static const int screenHeight = 500;

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

static unsigned short int dialogueState;

static unsigned char pressXAlpha;
bool alphaChangeDir;
static Shader textShader;
static int textBgLoc;

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
static void UpdateDrawFrame(void);      // Update and Draw one frame
static bool DrawButton(Vector2 position, float width, float height, std::string text);
static void DrawCenteredText(int y, int fontSize, std::string text, Color col = BLACK);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{

    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "raylib 9yr gamejam");
    
    // TODO: Load resources / Initialize variables at this point
    gameState = SCREEN_GAMEPLAY;
    gameplayState = HOOK_SNAKE;

    cam = Camera2D{ Vector2{ 0.0f, 0.0f }, Vector2{0.0f, 0.0f}, 0.0f, 1.0f};

    InitApples();

    normalSnake = NormalSnake(Vector2{ 0.0f, 0.0f });

    InitGrid();
    snake = Snake(Vector2{ 64.0f, 100.0f });
    hookSnake = HookSnake(Vector2{ 32.0f, 100.0f });

    showInstructions = false;
    instructionsTex = LoadTexture("resources/textures/instructionsScreen.png");
    dialogueState = 6;
    pressXAlpha = 1.0f;
    alphaChangeDir = false;

    std::string vsFileName = "text.vs";
    std::string fsFileName = "text.fs";

#if defined(PLATFORM_WEB)
    vsFileName = "resources/shaders/textWeb.vs";
    fsFileName = "resources/shaders/textWeb.fs";
#endif

    textShader = LoadShader(vsFileName.c_str(), fsFileName.c_str());
    textBgLoc = GetShaderLocation(textShader, "bgTex");
    
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
    UnloadShader(textShader);

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

            if (IsKeyPressed(KEY_X) && dialogueState < 6)
            {
                dialogueState++;
            }

            Vector2 snakePos = snake.GetPosition();
            snakePos.x /= GRID_W;
            snakePos.y /= GRID_H;
            int tX = (int)snakePos.x;
            int tY = (int)snakePos.y;

            if (GetGridAt(Vector2{ (float)tX, (float)tY }) == 3)
            {
                hookSnake = HookSnake(snake.GetPosition());
                gameplayState = HOOK_SNAKE;
                dialogueState = 6;
            }

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

            if (IsKeyPressed(KEY_X))
            {
                dialogueState++;
            }

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

                BeginShaderMode(textShader);
                {
                    if (dialogueState == 0)
                    {
                        DrawCenteredText(100, 30, "Hi.");
                        DrawCenteredText(150, 20, "Press X to continue");
                    }
                    else if (dialogueState == 1)
                    {
                        DrawText("!", snake.GetPosition().x, snake.GetPosition().y - 33, 30, BLACK);
                    }
                    else if (dialogueState == 2)
                    {
                        DrawCenteredText(50, 20, "There's not enough");
                        DrawCenteredText(75, 20, "time for me to");
                        DrawCenteredText(100, 20, " explain who I am.");
                        DrawCenteredText(125, 10, "You're in an apple.");
                        DrawCenteredText(135, 10, "And there are apples chasing you.");
                    }
                    else if (dialogueState == 3)
                    {
                        DrawCenteredText(100, 10, "(Yes, the plot to this game");
                        DrawCenteredText(110, 10, "really is that ridiculous)");
                    }
                    else if (dialogueState == 4)
                    {
                        DrawCenteredText(30, 20, "Use the arrow keys");
                        DrawCenteredText(55, 20, " or WASD to move.");
                        DrawCenteredText(80, 20, "Press up,");
                        DrawCenteredText(105, 20, "or W,");
                        DrawCenteredText(130, 20, "or space,");
                        DrawCenteredText(155, 20, " or Z, to jump.");
                        DrawCenteredText(180, 10, "Any of them work.");
                    }
                    else if (dialogueState == 5)
                    {
                        DrawCenteredText(75, 10, "The apples will be coming soon.");
                        DrawCenteredText(100, 20, "Better get a move on.");
                    }
                }
                EndShaderMode();

                break;
            }
            case HOOK_SNAKE:
            {
                DrawGrid();
                hookSnake.Draw();

                rlEnableShader(textShader.id);
                int slot = 5;
                rlActiveTextureSlot(slot);
                rlEnableTexture(target.texture.id);
                rlSetUniform(textBgLoc, &slot, SHADER_UNIFORM_INT, 1);
                rlActiveTextureSlot(0);
                rlDisableShader();
                BeginShaderMode(textShader);
                {
                    if (dialogueState == 6)
                    {
                        DrawCenteredText(100, 20, "New ability unlocked!");
                        DrawCenteredText(125, 10, "Press X to continue");
                    }
                    else if (dialogueState == 7)
                    {
                        DrawCenteredText(75, 20, "You can now control");
                        DrawCenteredText(100, 20, "Your tail! Move");
                        DrawCenteredText(125, 20, "around your mouse!");
                    }
                    else if (dialogueState == 8)
                    {
                        DrawCenteredText(100, 10, "(Remember, you can use WASD to move around)");
                    }
                    else if (dialogueState == 9)
                    {
                        DrawCenteredText(75, 10, "Hold down the left mouse button");
                        DrawCenteredText(85, 10, "while your tail is near a hook");
                        DrawCenteredText(95, 10, "to latch onto it.");
                        DrawCenteredText(120, 20, "Try it out here!");
                    }
                }
                EndShaderMode();

                break;
            }
            default:
            {
                break;
            }
            }
        }
        EndMode2D();

        if ((gameplayState == SNAKE && dialogueState < 6) || (gameplayState == HOOK_SNAKE))
        {
            DrawText("Press X", 256.0f - MeasureText("Press X", 10) - 3.0f, 5, 10, Color{ 0, 0, 0, pressXAlpha });
            if (!alphaChangeDir)
            {
                pressXAlpha -= 5;
                if (pressXAlpha <= 0)
                {
                    alphaChangeDir = true;
                    pressXAlpha = 0;
                }
            }
            else
            {
                pressXAlpha += 5;
                if (pressXAlpha >= 255)
                {
                    pressXAlpha = 255;
                    alphaChangeDir = false;
                }
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

static void DrawCenteredText(int y, int fontSize, std::string text, Color col)
{
    int textWidth = MeasureText(text.c_str(), fontSize);
    DrawText(text.c_str(), (256.0f / 2.0f) - (textWidth / 2.0f), y, fontSize, col);
}
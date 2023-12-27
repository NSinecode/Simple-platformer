#include <iostream>
#include <string>

#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include "Objects.h"

#define DEBUG

int main()
{
    const int screenWidth = GetMonitorWidth(GetCurrentMonitor());
    const int screenHeight = GetMonitorHeight(GetCurrentMonitor());

    InitWindow(screenWidth, screenHeight, "Game");
    SetWindowPosition(0, 30);

    InitAudioDevice();      // Initialize audio device

    //Camera settings
    Camera2D camera = { 0 };
    camera.offset = { (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 };
    camera.target = { 0, 0 };
    camera.zoom = 1.0f;

    //Player
    Player main;

    //Chapter
    Chapter first;
    first.setRecObstacles({ { -1000, -100, 2000, 100 }, { -2000, -200, 300, 200 }, { -1000, 600, 2000, 100 } , { 1000, -300, 100, 600 } , { -2100, -300, 100, 400 } });

    // Set our game to run at some frames-per-second
    SetTargetFPS(144);                  

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // Translate based on mouse right click
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            Vector2 delta = GetMouseDelta();
            delta = Vector2Scale(delta, -1.0f / camera.zoom);

            camera.target = Vector2Add(camera.target, delta);
        }

        // Zoom based on mouse wheel
        float wheel = GetMouseWheelMove();
        if (wheel != 0)
        {
            // Get the world point that is under the mouse
            Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

            // Set the offset to where the mouse is
            camera.offset = GetMousePosition();

            // Set the target to match, so that the camera maps the world space point 
            // under the cursor to the screen space point under the cursor at any zoom
            camera.target = mouseWorldPos;

            // Zoom increment
            const float zoomIncrement = 0.125f;

            camera.zoom += (wheel * zoomIncrement);
            if (camera.zoom < zoomIncrement) camera.zoom = zoomIncrement;
        }

        //Updating player
        UpdatePlayerMovement(main, first);

#ifdef DEBUG
        if (IsKeyPressed(KEY_R))
        {
            main.setPos({ 0,0 });
            main.setV({ 0,0 });
        }
#endif // DEBUG
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(WHITE);

        BeginMode2D(camera);

#ifdef DEBUG
        DrawCircle(0, 0, 50, GREEN);
        for (int i = 0; i < first.getRecObstacles().size(); i++)    DrawRectangleRecOrdinary(first.getRecObstacles()[i], RED);
        DrawRectangleRecOrdinary(main.getHitbox(), BLACK);
#endif // DEBUG

        EndMode2D();

#ifdef DEBUG
        DrawFPS(15, 40);
        DrawText(std::to_string(main.getV().x).c_str(), 15, 70, 20, GREEN);
        DrawText(std::to_string(main.getV().y).c_str(), 15, 100, 20, GREEN);
#endif // DEBUG


        EndDrawing();
    }


    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseAudioDevice();     // Close audio device
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    return 0;
}


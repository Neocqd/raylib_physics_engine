#include "raylib.h"
#include "Ball.h"

int main()
{
    // Initialization
    const int screenWidth  = 800;
    const int screenHeight = 450;
    
    InitWindow(screenWidth, screenHeight, "Raylib - Ramp, Gravity, & Flick Example");

    // Create a ball in the middle
    Ball ball(
        static_cast<float>(screenWidth) / 2.0f,
        static_cast<float>(screenHeight) / 2.0f,
        25.0f,
        0.98f  // friction
    );

    // Define a ramp as a line segment
    // For example: going from bottom-left to upper-right
    Vector2 rampStart = { 200.0f, 300.0f };
    Vector2 rampEnd   = { 500.0f, 200.0f };

    // Gravity in pixels/s^2
    float gravity = 400.0f;

    bool isDragging        = false;
    Vector2 previousMousePos = { 0.0f, 0.0f };

    SetTargetFPS(60);

    // Main loop
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();
        Vector2 mousePos = GetMousePosition();

        // Dragging / Flick logic
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (ball.IsMouseOver())
            {
                isDragging = true;
                // Reset velocity while dragging
                ball.velocity = { 0.0f, 0.0f };
            }
        }

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            if (isDragging)
            {
                // Flick
                Vector2 dragDelta = { mousePos.x - previousMousePos.x,
                                      mousePos.y - previousMousePos.y };
                // Multiply for stronger flick
                ball.velocity.x = dragDelta.x * 2.0f;
                ball.velocity.y = dragDelta.y * 2.0f;
            }
            isDragging = false;
        }

        // Update
        if (isDragging)
        {
            ball.position = mousePos;
        }
        else
        {
            // Apply physics (gravity, friction, collisions)
            ball.Update(gravity, deltaTime, screenWidth, screenHeight, rampStart, rampEnd);
        }

        // Store mouse pos for next frame
        previousMousePos = mousePos;

        // Draw
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            DrawText("Drag & release the ball for a flick. Watch it bounce on the ramp!", 
                     10, 10, 20, DARKGRAY);

            // Draw the ramp (line)
            DrawLineEx(rampStart, rampEnd, 4.0f, DARKGREEN);
            DrawCircleV(rampStart, 5.0f, RED);  // Mark ramp start
            DrawCircleV(rampEnd,   5.0f, RED);  // Mark ramp end

            ball.Draw();
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

#ifndef BALL_H
#define BALL_H

#include "raylib.h"

class Ball
{
public:
    // Core parameters
    Vector2 position; 
    Vector2 velocity; 
    float radius;     

    // Friction (0.0 ~ 1.0)
    float friction;

    // Constructor
    Ball(float x, float y, float r, float frictionCoeff);

    // Update logic:
    //  - gravity: downward acceleration (pixels/s^2)
    //  - deltaTime: frame time
    //  - screenWidth, screenHeight: used for window border collisions
    //  - rampStart, rampEnd: define the ramp as a line segment
    void Update(float gravity, float deltaTime, int screenWidth, int screenHeight,
                Vector2 rampStart, Vector2 rampEnd);

    // Draw the ball
    void Draw() const;

    // Check if the mouse is over the ball
    bool IsMouseOver() const;
};

#endif // BALL_H

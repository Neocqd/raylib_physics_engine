#include "Ball.h"
#include <cmath> // for std::fabs

// A helper function to reflect a vector v about a normalized normal n:
// reflection = v - 2 * (v • n) * n
static Vector2 ReflectVector(Vector2 v, Vector2 n)
{
    // Dot product
    float dot = v.x * n.x + v.y * n.y;    
    return {
        v.x - 2.0f * dot * n.x,
        v.y - 2.0f * dot * n.y
    };
}

// A helper function to get the closest point on a line segment from A->B to point P
static Vector2 ClosestPointOnLineSegment(Vector2 A, Vector2 B, Vector2 P)
{
    // 1. Compute the vector AB
    Vector2 AB = { B.x - A.x, B.y - A.y };
    // 2. Compute the vector AP
    Vector2 AP = { P.x - A.x, P.y - A.y };

    // 3. Project AP onto AB to find t, the parameter on AB
    float abLenSquared = AB.x * AB.x + AB.y * AB.y;
    if (abLenSquared < 0.000001f) 
    {
        // A and B are (nearly) the same point
        return A;
    }

    float dotABAP = (AP.x * AB.x + AP.y * AB.y);
    float t = dotABAP / abLenSquared;

    // 4. Clamp t to [0, 1] to stay within the segment
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    // 5. Compute the closest point: A + t * AB
    Vector2 closest = {
        A.x + t * AB.x,
        A.y + t * AB.y
    };
    return closest;
}

// Checks if the ball is colliding with the ramp (line segment).
// If there is a collision, returns true and also returns the ramp's normalized surface normal
// so we can reflect the velocity.
static bool CheckCollisionCircleLineSegment(Vector2 center, float radius,
                                            Vector2 lineStart, Vector2 lineEnd,
                                            Vector2 &outNormal)
{
    // 1. Find the closest point on the line segment to the center of the ball
    Vector2 closest = ClosestPointOnLineSegment(lineStart, lineEnd, center);

    // 2. Check distance from that closest point to the ball center
    Vector2 distVec = { center.x - closest.x, center.y - closest.y };
    float distSq = distVec.x * distVec.x + distVec.y * distVec.y;
    float rSq    = radius * radius;

    if (distSq <= rSq)
    {
        // We have a collision or are just touching the ramp
        // The normal is from the line to the center
        float dist = std::sqrt(distSq);
        if (dist < 0.00001f)
        {
            // The ball center is (extremely) close to the line's closest point
            // We'll define an arbitrary normal. Let's pick something non-zero, e.g. up:
            outNormal = { 0.0f, -1.0f };
        }
        else
        {
            outNormal = { distVec.x / dist, distVec.y / dist };
        }
        return true;
    }
    return false;
}

// Constructor
Ball::Ball(float x, float y, float r, float frictionCoeff)
{
    position = { x, y };
    velocity = { 0.0f, 0.0f };
    radius   = r;
    friction = frictionCoeff;
}

void Ball::Update(float gravity, float deltaTime, int screenWidth, int screenHeight,
                  Vector2 rampStart, Vector2 rampEnd)
{
    // 1. Gravity
    velocity.y += gravity * deltaTime;

    // 2. Apply friction (damping)
    velocity.x *= friction;
    velocity.y *= friction;

    // 3. Update position
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;

    // 4. Window border collisions
    // Left/right
    if (position.x < radius)
    {
        position.x = radius;
        velocity.x = -velocity.x;
    }
    else if (position.x > screenWidth - radius)
    {
        position.x = screenWidth - radius;
        velocity.x = -velocity.x;
    }

    // Top/bottom
    if (position.y < radius)
    {
        position.y = radius;
        velocity.y = -velocity.y;
    }
    else if (position.y > screenHeight - radius)
    {
        position.y = screenHeight - radius;
        velocity.y = -velocity.y;
    }

    // 5. Ramp collision
    Vector2 normal;
    if (CheckCollisionCircleLineSegment(position, radius, rampStart, rampEnd, normal))
    {
        // Reflect velocity about the normal
        velocity = ReflectVector(velocity, normal);

        // Push the ball out so it doesn't stay "inside" the ramp
        // We'll move out by just enough to get out of collision
        // Distance to ramp is radius - dist, we already have normal from line to center
        // We'll be a bit approximate to avoid re-checking distances:
        float distToMove = 1.0f; // You could compute it more precisely if needed
        position.x += normal.x * distToMove;
        position.y += normal.y * distToMove;
    }
}

void Ball::Draw() const
{
    DrawCircleV(position, radius, MAROON);
}

bool Ball::IsMouseOver() const
{
    return CheckCollisionPointCircle(GetMousePosition(), position, radius);
}

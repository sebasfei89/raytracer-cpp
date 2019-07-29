#include "SampleUtils.h"

#include <RayTracer/Canvas.h>
#include <RayTracer/Tuple.h>

#include <iostream>

struct Projectil
{
    Projectil(Tuple const& pos, Tuple const& velocity)
        : m_position(pos), m_velocity(velocity)
    {}

    Tuple m_position;
    Tuple m_velocity;
};

struct Environment
{
    Environment(Tuple const& gravity, Tuple const& wind)
        : m_gravity(gravity), m_wind(wind)
    {}

    Tuple m_gravity;
    Tuple m_wind;
};

void Tick(Environment const& env, Projectil& proj)
{
    proj.m_position = proj.m_position + proj.m_velocity;
    proj.m_velocity = proj.m_velocity + env.m_gravity + env.m_wind;
}

int main()
{
    Tuple velocity = Vector(1.f, 1.8f, 0.f);
    velocity.Normalize();

    Projectil proj(Point(0.f, 0.f, 0.f), velocity * 11.25);
    Environment env(Vector(0.f, -0.1f, 0.f), Vector(-0.01f, 0.f, 0.f));
    Canvas canvas(900, 550);
    uint32_t const yBias = canvas.Height() - 1;
    std::cout << "Environment => Gravity: " << env.m_gravity << " Wind: " << env.m_wind << std::endl;
    do
    {
        std::cout << "Projectil => Position: " << proj.m_position << " Velocity: " << proj.m_velocity << std::endl;
        canvas.WritePixel((int)proj.m_position[0], yBias - (int)proj.m_position[1], Color(1.f, 0.f, 0.f));
        Tick(env, proj);
    } while (proj.m_position[1] > 0.0f);

    SampleUtils::SaveCanvas(canvas, "projectil_trajectory.ppm");

    return 0;
}

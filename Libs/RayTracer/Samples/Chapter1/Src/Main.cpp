#include "Tuple.h"

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
    Projectil proj(Point(2.f, 0.f, 3.f), Vector(10.f, 50.f, 10.f));
    Environment env(Vector(0.f, -9.8f, 0.f), Vector(1.f, 0.f, 1.f));

    std::cout << "Environment => Gravity: " << env.m_gravity << " Wind: " << env.m_wind << std::endl;
    do
    {
        std::cout << "Projectil => Position: " << proj.m_position << " Velocity: " << proj.m_velocity << std::endl;
        Tick(env, proj);
    } while (proj.m_position[1] > 0.0f);

    return 0;
}

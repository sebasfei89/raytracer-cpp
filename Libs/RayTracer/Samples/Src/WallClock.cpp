#include <Canvas.h>
#include <Transformations.h>

#include <fstream>
#include <iostream>

int main()
{
    Canvas canvas(400, 400);
    Color const white(1.f, 1.f, 1.f);
    float const rotationAngle = PI / 6.f;
    Mat44 const translate = matrix::Translation(200.f, 200.f, 0.f);
    Mat44 const scale = matrix::Scaling(150.f, 150.f, 1.f);
    Mat44 const tx = translate * scale;

    // Draw clock center
    canvas.WritePixel(200, 200, Color(1.f, 0.f, 0.f));

    Tuple twelveOclock = Point(0.f, 1.f, 0.f);
    for (auto h : { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 })
    {
        Mat44 const rotate = matrix::RotationZ((h) * -rotationAngle);
        
        Tuple const actualHour = tx * rotate * twelveOclock;
        canvas.WritePixel((uint32_t)actualHour[0], canvas.Height() - (uint32_t)actualHour[1], white);
    }

    std::ofstream ppmFile;
    ppmFile.open("wall_clock.ppm");
    ppmFile << canvas.GetAsPPM();
    ppmFile.close();

    return 0;
}

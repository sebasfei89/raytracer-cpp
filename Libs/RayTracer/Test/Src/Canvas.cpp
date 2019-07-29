#include <RayTracer/Canvas.h>

#include <Beddev/Beddev.h>

SCENARIO("Creating a canvas", "rendering")
{
    GIVEN( Canvas const c(2, 3)
         , Color const black(0.f, 0.f, 0.f) )
    THEN( c.Width() == 2
        , c.Height() == 3
        , c.PixelAt(0, 0) == black
        , c.PixelAt(0, 1) == black
        , c.PixelAt(0, 2) == black
        , c.PixelAt(1, 0) == black
        , c.PixelAt(1, 1) == black
        , c.PixelAt(1, 2) == black )
}

SCENARIO("Writing pixels to a canvas", "rendering")
{
    
    GIVEN( Canvas c(10, 20)
         , Color const red(1.0f, 0.0f, 0.0f) )
    WHEN(c.WritePixel(2, 3, red))
    THEN(c.PixelAt(2, 3) == red)
}

SCENARIO("Writing a pixel outside the boundaries of a canvas it's a NOOP", "rendering")
{
    GIVEN( Canvas c(2, 2) )
    WHEN( c.WritePixel(1, 3, Color(1.f, 0.f, 0.f)) )
    THEN( true )
}

SCENARIO("Constructing the PPM header", "rendering")
{
    GIVEN( Canvas const c(5, 3) )
    WHEN( auto const ppm = c.GetAsPPM() )
    THEN( ppm.substr(0, 11) == "P3\n5 3\n255\n" )
}

SCENARIO("Constructing the PPM pixel data", "rendering")
{
    constexpr char const* expected =
        "P3\n"
        "5 3\n"
        "255\n"
        "255 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n"
        "0 0 0 0 0 0 0 128 0 0 0 0 0 0 0\n"
        "0 0 0 0 0 0 0 0 0 0 0 0 0 0 255\n";

    GIVEN( Canvas c(5, 3)
         , c.WritePixel(0, 0, Color(1.5f, 0.0f, 0.0f))
         , c.WritePixel(2, 1, Color(0.0f, 0.5f, 0.0f))
         , c.WritePixel(4, 2, Color(-0.5f, 0.0f, 1.0f)) )
    WHEN( auto const ppm = c.GetAsPPM() )
    THEN( ppm == expected )
}

SCENARIO("Splitting long lines in PPM files", "rendering")
{
    constexpr char const* expected = "P3\n10 2\n255\n"
        "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n"
        "153 255 204 153 255 204 153 255 204 153 255 204 153\n"
        "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n"
        "153 255 204 153 255 204 153 255 204 153 255 204 153\n";

    GIVEN( Canvas c(10, 2)
         , Color const color(1.f, 0.8f, 0.6f)
         , for (int i = 0; i < c.Width(); i++)
               for (int j = 0; j < c.Height(); j++)
                   c.WritePixel(i, j, color) )
    WHEN( std::string const ppm = c.GetAsPPM() )
    THEN( ppm == expected )
}

SCENARIO("PPM files are terminated by a newline character", "rendering")
{
    GIVEN( Canvas const c(5, 3) )
    WHEN( auto const ppm = c.GetAsPPM() )
    THEN( ppm.at(ppm.size() - 1) == '\n' )
}

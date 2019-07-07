#include "Canvas.h"

#include <Testing.h>

SCENARIO("Creating a canvas", "[rendering]")
{
    GIVEN("A canvas created with size=(10, 20)")
    {
        Canvas const c(10, 20);
        Color const black(0.f, 0.f, 0.f);

        THEN_REQUIRE(c.Width() == 10)
        AND_THEN_REQUIRE(c.Height() == 20)

        for (int i = 0; i < c.Width(); i++)
        {
            for (int j = 0; j < c.Height(); j++)
            {
                AND_THEN_REQUIRE(c.PixelAt(i, j) == black);
            }
        }
    }
}

SCENARIO("Writing pixels to a canvas", "[rendering]")
{
    GIVEN("A fresh canvas and a red color")
    {
        Canvas c(10, 20);
        Color const red(1.0f, 0.0f, 0.0f);

        WHEN_REQUIRE(c.WritePixel(2, 3, red), c.PixelAt(2, 3) == red)

        WHEN("We write a pixel outside the boundaries of a canvas it's a NOOP")
        {
            REQUIRE_NOTHROW(c.WritePixel(2000, 3, red));
        }
    }
}

SCENARIO("Constructing the PPM header", "[rendering]")
{
    GIVEN_WHEN_REQUIRE(
        Canvas const c(5, 3),
        std::string const ppm = c.GetAsPPM(),
        ppm.substr(0, 11) == "P3\n5 3\n255\n")
}

SCENARIO("Constructing the PPM pixel data", "[rendering]")
{
    GIVEN("A canvas of size (5, 3) with some pixels colored")
    {
        Canvas c(5, 3);
        c.WritePixel(0, 0, Color(1.5f, 0.0f, 0.0f));
        c.WritePixel(2, 1, Color(0.0f, 0.5f, 0.0f));
        c.WritePixel(4, 2, Color(-0.5f, 0.0f, 1.0f));

        constexpr char const* expected = "P3\n5 3\n255\n"
            "255 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n"
            "0 0 0 0 0 0 0 128 0 0 0 0 0 0 0\n"
            "0 0 0 0 0 0 0 0 0 0 0 0 0 0 255\n";

        WHEN_REQUIRE(std::string const ppm = c.GetAsPPM(), ppm == expected)
    }
}

SCENARIO("Splitting long lines in PPM files", "[rendering]")
{
    GIVEN("A canvas of size (10, 2) with every pixel set to [1, 0.8, 0.6]")
    {
        Canvas c(10, 2);
        Color const color(1.f, 0.8f, 0.6f);
        for (int i = 0; i < c.Width(); i++)
        {
            for (int j = 0; j < c.Height(); j++)
            {
                c.WritePixel(i, j, color);
            }
        }

        constexpr char const* expected = "P3\n10 2\n255\n"
            "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n"
            "153 255 204 153 255 204 153 255 204 153 255 204 153\n"
            "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n"
            "153 255 204 153 255 204 153 255 204 153 255 204 153\n";

        WHEN_REQUIRE(std::string const ppm = c.GetAsPPM(), ppm == expected)
    }
}

SCENARIO("PPM files are terminated by a newline character", "[rendering]")
{
    GIVEN_WHEN_REQUIRE(
        Canvas const c(5, 3),
        auto const ppm = c.GetAsPPM(),
        ppm.at(ppm.size() - 1) == '\n')
}

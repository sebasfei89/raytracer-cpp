#include "Canvas.h"

#include "Catch2/catch.hpp"

SCENARIO("Creating a canvas", "[rendering]")
{
    GIVEN("A canvas created with size=(10, 20)")
    {
        Canvas c(10, 20);

        THEN("Canvas size is (10, 20)")
        {
            REQUIRE(c.Width() == 10);
            REQUIRE(c.Height() == 20);
        }
        AND_THEN("Every pixel is black")
        {
            for (int i = 0; i < c.Width(); i++)
            {
                for (int j = 0; j < c.Height(); j++)
                {
                    REQUIRE(c.PixelAt(i, j) == Color(0.f, 0.f, 0.f));
                }
            }
        }
    }
}

SCENARIO("Writing pixels to a canvas")
{
    GIVEN("A fresh canvas and a red color")
    {
        Canvas c(10, 20);
        Color const red(1.0f, 0.0f, 0.0f);
        WHEN("We write a red pixel on canvas at position (2,3)")
        {
            c.WritePixel(2, 3, red);
            THEN("Canvas pixel at (2,3) is red")
            {
                REQUIRE(c.PixelAt(2, 3) == Color(1.f, 0.f, 0.f));
            }
        }

        WHEN("We write a pixel outside the boundaries of a canvas it's a NOOP")
        {
            REQUIRE_NOTHROW(c.WritePixel(2000, 3, red));
        }
    }
}

SCENARIO("Constructing the PPM header")
{
    GIVEN("A fresh canvas of size (5, 3)")
    {
        Canvas const c(5, 3);
        WHEN("We export the canvas to PPM")
        {
            std::string const ppm = c.GetAsPPM();
            THEN("PPM header is written successfully")
            {
                REQUIRE(ppm.substr(0, 11) == "P3\n5 3\n255\n");
            }
        }
    }
}

SCENARIO("Constructing the PPM pixel data")
{
    GIVEN("A canvas of size (5, 3) with some pixels colored")
    {
        Canvas c(5, 3);
        c.WritePixel(0, 0, Color(1.5f, 0.0f, 0.0f));
        c.WritePixel(2, 1, Color(0.0f, 0.5f, 0.0f));
        c.WritePixel(4, 2, Color(-0.5f, 0.0f, 1.0f));
        WHEN("We export the canvas to PPM")
        {
            std::string const ppm = c.GetAsPPM();
            THEN("PPM pixel data is written successfully")
            {
                constexpr char const* expected = "P3\n5 3\n255\n"
                    "255 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n"
                    "0 0 0 0 0 0 0 128 0 0 0 0 0 0 0\n"
                    "0 0 0 0 0 0 0 0 0 0 0 0 0 0 255\n";
                REQUIRE(ppm == expected);
            }
        }
    }
}

SCENARIO("Splitting long lines in PPM files")
{
    GIVEN("A canvas of size (10, 2) with every pixel set to [1, 0.8, 0.6]")
    {
        Canvas c(10, 2);
        Color color(1.f, 0.8f, 0.6f);
        for (int i = 0; i < c.Width(); i++)
        {
            for (int j = 0; j < c.Height(); j++)
            {
                c.WritePixel(i, j, color);
            }
        }
        WHEN("We export the canvas to PPM")
        {
            std::string const ppm = c.GetAsPPM();
            THEN("PPM pixel data lines do not exceed 70 characters")
            {
                constexpr char const* expected = "P3\n10 2\n255\n"
                    "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n"
                    "153 255 204 153 255 204 153 255 204 153 255 204 153\n"
                    "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n"
                    "153 255 204 153 255 204 153 255 204 153 255 204 153\n";
                REQUIRE(ppm == expected);
            }
        }
    }
}

SCENARIO("PPM files are terminated by a newline character")
{
    GIVEN("A canvas of size (5, 3)")
    {
        Canvas const c(5, 3);
        WHEN("We export the canvas to PPM")
        {
            std::string const ppmStr = c.GetAsPPM();
            THEN("PPM ends with a newline character")
            {
                REQUIRE(ppmStr.at(ppmStr.size() - 1) == '\n');
            }
        }
    }
}

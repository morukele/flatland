
#pragma once

#include <random>
#include <string>

namespace flatland
{
    enum class Colour
    {
        WHITE,
        BLUE,
        YELLOW,
        BLACK,
        RED,
        GREEN,
        PURPLE
    };


    /*
     * A function to covert the Enum to a string.
     * NB: it does not modify the colour value
     */
    inline std::string convertColourEnumToText(Colour c)
    {
        switch (c)
        {
        case Colour::WHITE:
            return "WHITE";
        case Colour::BLUE:
            return "BLUE";
        case Colour::YELLOW:
            return "YELLOW";
        case Colour::BLACK:
            return "BLACK";
        case Colour::RED:
            return "RED";
        case Colour::GREEN:
            return "GREEN";
        case Colour::PURPLE:
            return "PURPLE";
        default:
            return "UNKNOWN";
        }
    }

    static int clamp(int value, int max, int min)
    {
        if (value < min)
            return min;
        if (value > max)
            return min;
        return value;
    }

    /**
     * A function to return a random generator.
     * @return std::mt19937
     */
    inline std::mt19937 &rng()
    {
        static std::mt19937 gen{std::random_device{}()};
        return gen;
    }

    /**
     * A function to generate a random number between an upper and a lower limit
     * @param lo lower limit
     * @param hi upper limit
     * @return returns a random int between the low and upper limit inclusive
     */
    inline int randInt(const int lo, const int hi)
    {
        return std::uniform_int_distribution(lo, hi)(rng());
    }

    // A function that returns a random enum variant
    inline Colour randomColour()
    {
        return static_cast<Colour>(randInt(0, 6));
    }
} // namespace flatland


#pragma once

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
} // namespace flatland

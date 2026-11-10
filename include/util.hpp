
#pragma once

#include <random>
#include <string>
#include <stdexcept>

namespace flatland
{
    /**
     * Custom exception for invalid integer arguments.
     * Extends std::invalid_argument to provide access to the invalid value
     * and the permitted min/max range, allowing callers to correct the value.
     */
    class invalid_intarg : public std::invalid_argument {
    public:
        invalid_intarg(int invalid_value, int min_val, int max_val, const std::string& param_name);
        int getInvalidValue() const { return invalid_value; }
        int getMin() const { return min_val; }
        int getMax() const { return max_val; }
        const char* what() const noexcept override { return message.c_str(); }
    private:
        int invalid_value;
        int min_val;
        int max_val;
        std::string message;
    };

    inline invalid_intarg::invalid_intarg(int invalid_value, int min_val, int max_val, const std::string& param_name)
        : std::invalid_argument("")
        , invalid_value(invalid_value)
        , min_val(min_val)
        , max_val(max_val)
        , message(param_name + " value " + std::to_string(invalid_value) + " is out of range [" + std::to_string(min_val) + ", " + std::to_string(max_val) + "]")
    {
    }
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
            return max;
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

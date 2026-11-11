#pragma once
#include <fstream>
#include <string>
#include "flatlandien.hpp"

namespace flatland
{
    // =========================================================
    // A stream class to write a flatlander to file.
    // =========================================================
    class FlatOutputStream : public std::ofstream
    {
    public:
        explicit FlatOutputStream(const std::string& filename);
        void write(const Flatlander &f);
    };

    // =========================================================
    // A stream class to read saved flatlander from file.
    // =========================================================
    class FlatInputStream : public std::ifstream
    {
    public:
        explicit FlatInputStream(const std::string& filename);
        /**
         * A function to read flatlanders from a file.
         * @return unique pointer to Flatlander object or nullptr if not able to read
         */
        std::unique_ptr<Flatlander> read();
    };

}
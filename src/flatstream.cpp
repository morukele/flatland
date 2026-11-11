/**
* @file flatstream
* @author oghenemarho
* @date 11/11/2026
* @project flatland
*
*/
#include "flatstream.hpp"
#include <iomanip>

namespace flatland
{
    FlatInputStream::FlatInputStream(const std::string& filename) : std::ifstream(filename)
    {
    }

    FlatOutputStream::FlatOutputStream(const std::string& filename) : std::ofstream(filename)
    {
    }

    void FlatOutputStream::write(const Flatlander& f)
    {
        int selector = -1;
        int extra = 0;

        if (const auto* i = dynamic_cast<const Isocele*>(&f))
        {
            selector = 1;
            extra = i->getPeakAngle();
        }
        else if (const auto* p = dynamic_cast<const Polygon*>(&f))
        {
            selector = 2;
            extra = p->getNbSides();
        }
        else if (dynamic_cast<const Segment*>(&f))
        {
            selector = 0;
        }
        else
        {
            throw std::logic_error("Unknown Flatlander type");
        }

        *this << selector << ' ' << std::quoted(f.getName()) << ' '
            << f.getSize() << ' ' << static_cast<int>(f.getColour()) << ' '
            << f.isAlive();

        if (selector != 0)
        {
            *this << ' ' << extra;
        }
        *this << '\n';
    }

    std::unique_ptr<Flatlander> FlatInputStream::read()
    {
        int selector;
        if (!(*this >> selector))
        {
            return nullptr; // end of file
        }

        std::string name;
        int size, colour, extra = 0;
        bool alive;
        *this >> std::quoted(name) >> size >> colour >> alive;
        if (selector == 1 || selector == 2)
        {
            *this >> extra;
        }
        if (!*this)
        {
            return nullptr; // truncated or corrupt record
        }

        std::unique_ptr<Flatlander> result;
        const auto c = static_cast<Colour>(colour);
        switch (selector)
        {
        case 0:
            result = std::make_unique<Segment>(name, size, c);
            break;
        case 1:
            {
                auto i = std::make_unique<Isocele>(name, size, c);
                RobotSurgeon::modifyPeakAngle(*i, extra); // restore the saved angle
                result = std::move(i);
                break;
            }
        case 2:
            result = std::make_unique<Polygon>(name, size, c, extra);
            break;
        default:
            return nullptr; // unknown selector
        }

        if (!alive)
        {
            result->kill();
        }
        return result;
    }
}

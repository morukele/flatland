#include "flatlandien.hpp"

#include <iostream>

namespace flatland
{
    /*
     * Flatlander Implementation
     */
    Flatlander::Flatlander(std::string n, int s, Colour c) : name{n}, size{s}, colour{c} {}
    std::string Flatlander::getName() { return name; }
    void Flatlander::setName(std::string n) { name = n; }
    int Flatlander::getSize() { return size; }
    void Flatlander::setSize(int s) { size = clamp(s, MAX_SIZE, MIN_SIZE); }
    Colour Flatlander::getColour() { return colour; }

    /*
     * Segment Implementation
     */
    Segment::Segment(std::string n, int s, Colour c) : Flatlander(n, s, c)
    {
        ++numberOfSegments;
    }

    void Segment::present()
    {
        std::cout << "Greeting from Segment -> " << getName() << "with size -> " << getSize() << " with colour -> " << convertColourEnumToText(getColour()) << std::endl;
    }

    /*
     * Isocele Implementation
     */
    Isocele::Isocele(std::string n, int s, Colour c) : Flatlander(n, s, c)
    {
        ++numberOfIsocele;
    }

    int Isocele::getPeakAngle() { return peakAngle; }
    void Isocele::present()
    {
        std::cout << "Greeting from Isocele -> " << getName() << "with size -> " << getSize() << " with colour -> " << convertColourEnumToText(getColour()) << std::endl;
    }

    /*
     * Polygon Implementation
     */
    Polygon::Polygon(std::string n, int s, Colour c) : Flatlander(n, s, c)
    {
        ++numberOfPolygon;
    }

    int Polygon::getNbSides() { return nbSides; }
    void Polygon::present()
    {
        std::cout << "Greetings from Polygon -> " << getName() << " with size -> " << getSize() << " with colour -> " << convertColourEnumToText(getColour()) << std::endl;
    }

    /*
     * Robot Surgeon Implementation
     */
    RobotSurgeon::RobotSurgeon(std::string n) : name{n} {}

    std::string RobotSurgeon::getName() { return name; }
    void RobotSurgeon::modifyColour(Segment &s, Colour c) { s.colour = c; }
    void RobotSurgeon::modifyColour(Isocele &i, Colour c) { i.colour = c; }
    void RobotSurgeon::modifyColour(Polygon &p, Colour c) { p.colour = c; }
    void RobotSurgeon::modifyPeakAngle(Isocele &i, int a) { i.peakAngle = clamp(a, MAX_ANGLE, MIN_ANGLE); }
    void RobotSurgeon::modifyNbSides(Polygon &p, int n) { p.nbSides = clamp(n, MAX_nbSIZE, MIN_nbSIZE); }

    void printStatistics()
    {
        std::cout << "Number of Segments -> " << numberOfSegments << std::endl;
        std::cout << "Number of Isocele -> " << numberOfIsocele << std::endl;
        std::cout << "Number of Polygon -> " << numberOfPolygon << std::endl;
    }

    /*
     * Family Implementation
     */

} // namespace flatland

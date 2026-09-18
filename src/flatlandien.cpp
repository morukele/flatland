#include "flatlandien.hpp"

#include <iostream>

namespace flatland
{
    /*
     * Segment Implementation
     */
    Segment::Segment(std::string n, int s, Colour c) : name{n}, size{s}, colour{c}
    {
        ++numberOfSegments;
    }

    std::string Segment::getName() { return name; }
    void Segment::setName(std::string n) { name = n; }
    int Segment::getSize() { return size; }
    void Segment::setSize(int s) { size = clamp(s, MAX_SIZE, MIN_SIZE); }
    Colour Segment::getColor() { return colour; }
    void Segment::present()
    {
        std::cout << "Greeting from Segment -> " << this->name << "with size -> " << this->size << " with colour -> " << convertColourEnumToText(colour) << std::endl;
    }

    /*
     * Isocele Implementation
     */
    Isocele::Isocele(std::string n, int s, Colour c) : name{n}, size{s}, colour{c}
    {
        ++numberOfIsocele;
    }

    std::string Isocele::getName() { return name; }
    void Isocele::setName(std::string n) { name = n; }
    int Isocele::getSize() { return size; }
    void Isocele::setSize(int s) { size = clamp(s, MAX_SIZE, MIN_SIZE); }
    Colour Isocele::getColour() { return colour; }
    int Isocele::getPeakAngle() { return peakAngle; }
    void Isocele::present()
    {
        std::cout << "Greeting from Isocele -> " << this->name << "with size -> " << this->size << " with colour -> " << convertColourEnumToText(colour) << std::endl;
    }

    /*
     * Polygon Implementation
     */
    Polygon::Polygon(std::string n, int s, Colour c) : name{n}, size{s}, colour{c}
    {
        ++numberOfPolygon;
    }

    std::string Polygon::getName() { return name; }
    void Polygon::setName(std::string n) { name = n; }
    int Polygon::getSize() { return size; }
    void Polygon::setSize(int s) { size = clamp(s, MAX_SIZE, MIN_SIZE); }
    Colour Polygon::getColour() { return colour; }
    int Polygon::getNbSides() { return nbSides; }
    void Polygon::present()
    {
        std::cout << "Greetings from Polygon -> " << this->name << " with size -> " << this->size << " with colour -> " << convertColourEnumToText(colour) << std::endl;
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

} // namespace flatland

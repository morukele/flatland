#include "flatlandien.hpp"

#include <iostream>

namespace flatland
{
    /*
     * Segment Implementation
     */
    Segment::Segment(std::string n, int s, Colour c) : name{n}, size{s}, colour{c} {}

    std::string Segment::getName() { return name; }
    void Segment::setName(std::string n) { name = n; }
    int Segment::getSize() { return size; }
    void Segment::setSize(int s) { size = s; }
    Colour Segment::getColor() { return colour; }
    void Segment::present()
    {
        std::cout << "Greeting from Segment -> " << this->name << "with size -> " << this->size << " with colour -> " << convertColourEnumToText(colour) << std::endl;
    }

    /*
     * Isocele Implementation
     */
    Isocele::Isocele(std::string n, int s, Colour c) : name{n}, size{s}, colour{c} {}

    std::string Isocele::getName() { return name; }
    void Isocele::setName(std::string n) { name = n; }
    int Isocele::getSize() { return size; }
    void Isocele::setSize(int s) { size = s; }
    Colour Isocele::getColour() { return colour; }
    int Isocele::getPeakAngle() { return peakAngle; }
    void Isocele::present()
    {
        std::cout << "Greeting from Isocele -> " << this->name << "with size -> " << this->size << " with colour -> " << convertColourEnumToText(colour) << std::endl;
    }

    /*
     * Polygone Implementation
     */
    Polygone::Polygone(std::string n, int s, Colour c) : name{n}, size{s}, colour{c} {}

    std::string Polygone::getName() { return name; }
    void Polygone::setName(std::string n) { name = n; }
    int Polygone::getSize() { return size; }
    void Polygone::setSize(int s) { size = s; }
    Colour Polygone::getColour() { return colour; }
    int Polygone::getNbSides() { return nbSides; }
    void Polygone::present()
    {
        std::cout << "Greetings from Polygone -> " << this->name << "with size -> " << this->size << " with colour -> " << convertColourEnumToText(colour) << std::endl;
    }

    /*
     * Robot Surgeon Implementation
     */
    RobotSurgeon::RobotSurgeon(std::string n) : name{n} {}

    std::string RobotSurgeon::getName() { return name; }
    void RobotSurgeon::modifyColour(Segment &s, Colour c) { s.colour = c; }
    void RobotSurgeon::modifyColour(Isocele &i, Colour c) { i.colour = c; }
    void RobotSurgeon::modifyColour(Polygone &p, Colour c) { p.colour = c; }
    void RobotSurgeon::modifyPeakAngle(Isocele &i, int a) { i.peakAngle = a; }
    void RobotSurgeon::modifyNbSides(Polygone &p, int n) { p.nbSides = n; }

} // namespace flatland

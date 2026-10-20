#include "flatlandien.hpp"
#include <iostream>

namespace flatland
{
    /**
     * Flatlander Implementation
     */
    Flatlander::Flatlander(const std::string& n, const int s, const Colour c) : name{n}, size{s}, colour{c}
    {
    }

    std::string Flatlander::getName() const { return name; }
    void Flatlander::setName(const std::string& n) { name = n; }
    int Flatlander::getSize() const { return size; }
    void Flatlander::setSize(const int s) { size = clamp(s, MAX_SIZE, MIN_SIZE); }
    Colour Flatlander::getColour() const { return colour; }

    /**
     * Segment Implementation
     */
    Segment::Segment(const std::string& n, const int s, const Colour c)
        : Flatlander(n, s, c)
    {
        ++numberOfSegments;
    }

    void Segment::present() const
    {
        std::cout << "Greeting from Segment -> " << getName() << "with size -> " << getSize() << " with colour -> " <<
            convertColourEnumToText(getColour()) << std::endl;
    }

    /**
     * Isocele Implementation
     */
    Isocele::Isocele(const std::string& n, int s, Colour c) : Flatlander(n, s, c)
    {
        ++numberOfIsocele;
    }

    int Isocele::getPeakAngle() const { return peakAngle; }

    void Isocele::present() const
    {
        std::cout << "Greeting from Isocele -> " << getName() << "with size -> " << getSize() << " with colour -> " <<
            convertColourEnumToText(getColour()) << std::endl;
    }

    /**
     * Polygon Implementation
     */
    Polygon::Polygon(const std::string& n, int s, Colour c, int sides) : Flatlander(n, s, c), nbSides{sides}
    {
        ++numberOfPolygon;
    }

    int Polygon::getNbSides() const { return nbSides; }

    void Polygon::present() const
    {
        std::cout << "Greetings from Polygon -> " << getName() << " with size -> " << getSize() << " with colour -> " <<
            convertColourEnumToText(getColour()) << std::endl;
    }

    /**
     * Robot Surgeon Implementation
     */
    RobotSurgeon::RobotSurgeon(std::string n) : name{n}
    {
    }

    std::string RobotSurgeon::getName() const { return name; }
    void RobotSurgeon::modifyColour(Flatlander& flatlander, Colour c) { flatlander.colour = c; }
    void RobotSurgeon::modifyPeakAngle(Isocele& i, int a) { i.peakAngle = clamp(a, MAX_ANGLE, MIN_ANGLE); }
    void RobotSurgeon::modifyNbSides(Polygon& p, int n) { p.nbSides = clamp(n, MAX_NB_SIZE, MIN_NB_SIZE); }

    void printStatistics()
    {
        std::cout << "Number of Segments -> " << numberOfSegments << std::endl;
        std::cout << "Number of Isocele -> " << numberOfIsocele << std::endl;
        std::cout << "Number of Polygon -> " << numberOfPolygon << std::endl;
    }

    /**
     * Family Implementation
     */
    SingleChildFamily::SingleChildFamily(Father* f, Segment* m, Child* c) : father{f}, mother{m}, child{c}
    {
    }

    void SingleChildFamily::setFather(Father* f)
    {
        // check for null input passed in
        if (f != nullptr && father == nullptr)
        {
            father = f;
        }
        else
        {
            std::cerr << "Father is already set or null input passed in" << std::endl;
        }
    }

    void SingleChildFamily::setMother(Segment* m)
    {
        if (m != nullptr && mother == nullptr)
        {
            mother = m;
        }
        else
        {
            std::cerr << "Mother is already se or null input passed int" << std::endl;
        }
    }

    bool SingleChildFamily::setChild(Child* c)
    {
        // TODO: check if null input passed in
        if (child != nullptr)
        {
            std::cerr << "Child already exists in family" << std::endl;
            return false; // child already exists
        }
        child = c;
        return true; // setting child was successful
    }

    void SingleChildFamily::present() const
    {
        std::cout << std::endl;
        std::cout << "======= Printing family details =======" << std::endl;
        std::cout << "Father name -> " << father->getName() << std::endl;
        std::cout << "Mother name -> " << mother->getName() << std::endl;
        std::cout << "Child name -> " << child->getName() << std::endl;
    }

    std::unique_ptr<Flatlander> Flatlander::newFlatlander(const Segment& mother, const Father& father)
    {
        const std::string name = mother.getName() = "-" + father.getName(); // compound name from parents
        int size = clamp((mother.getSize() + father.getSize()) / 2, MIN_SIZE, MAX_SIZE);
        Colour colour = randInt(0, 1) == 0 ? mother.getColour() : father.getColour();

        // 50% female -> Segment
        if (randInt(0, 1) == 0)
        {
            return std::make_unique<Segment>(name, size, colour);
        }

        // Male depending on the father type
        if (dynamic_cast<const Isocele*>(&father))
        {
            if (randInt(0, 1) == 0)
            {
                return std::make_unique<Isocele>(name, size, colour); // should be an isoceles triangle
            }

            return std::make_unique<Polygon>(name, size, colour, 4); // should be a square
        }

        // Father is Polygon -> child is a polygon
        return std::make_unique<Polygon>(name, size, colour, randInt(MIN_NB_SIZE, MAX_NB_SIZE));
    }
} // namespace flatland

#include "flatlandien.hpp"
#include <iostream>

namespace flatland
{
    /**
     * Flatlander Implementation
     */
    Flatlander::Flatlander() : name{}, size{0}, colour{}
    {
    }

    Flatlander::Flatlander(const std::string& n, const int s, const Colour c)
        : name{n}, size{s}, colour{c}
    {
    }

    std::string Flatlander::getName() const { return name; }
    void Flatlander::setName(const std::string& n) { name = n; }
    int Flatlander::getSize() const { return size; }
    void Flatlander::setSize(const int s) { size = clamp(s, MAX_SIZE, MIN_SIZE); }
    Colour Flatlander::getColour() const { return colour; }
    void Flatlander::kill() { alive = false; }
    bool Flatlander::isAlive() const { return alive; }

    void Flatlander::present() const
    {
        std::cout << getName() << " (size " << getSize() << ", "
            << (isAlive() ? "alive" : "dead")
            << ", " << convertColourEnumToText(getColour())
            << ")";
    }

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
        std::cout << "[Segment] -> ";
        Flatlander::present();
        std::cout << std::endl;
    }

    /**
     * Isocele Implementation
     */
    Isocele::Isocele(const std::string& n, int s, Colour c) : FlatlanderMale(n, s, c)
    {
        peakAngle = randInt(MIN_ANGLE, MAX_ANGLE);
        ++numberOfIsocele;
    }

    int Isocele::getPeakAngle() const { return peakAngle; }

    void Isocele::present() const
    {
        std::cout << "[Isocele, peak angle " << getPeakAngle() << "] -> ";
        Flatlander::present();
        std::cout << std::endl;
    }

    /**
     * Polygon Implementation
     */
    Polygon::Polygon(const std::string& n, int s, Colour c, int sides) : FlatlanderMale(n, s, c), nbSides{sides}
    {
        ++numberOfPolygon;
    }

    int Polygon::getNbSides() const { return nbSides; }

    void Polygon::present() const
    {
        std::cout << "[Polygon, " << getNbSides() << " sides] -> ";
        Flatlander::present();
        std::cout << std::endl;
    }

    /**
     * Robot Surgeon Implementation
     */
    RobotSurgeon::RobotSurgeon(const std::string& n) : name{n}
    {
    }

    std::string RobotSurgeon::getName() const { return name; }
    void RobotSurgeon::modifyColour(Flatlander& flatlander, const Colour c) { flatlander.colour = c; }
    void RobotSurgeon::modifyPeakAngle(Isocele& i, const int a) { i.peakAngle = clamp(a, MAX_ANGLE, MIN_ANGLE); }
    void RobotSurgeon::modifyNbSides(Polygon& p, const int n) { p.nbSides = clamp(n, MAX_NB_SIZE, MIN_NB_SIZE); }

    void printStatistics()
    {
        std::cout << "Number of Segments -> " << numberOfSegments << std::endl;
        std::cout << "Number of Isocele -> " << numberOfIsocele << std::endl;
        std::cout << "Number of Polygon -> " << numberOfPolygon << std::endl;
    }

    /**
     * Family Implementation
     */
    SingleChildFamily::SingleChildFamily(FlatlanderMale* f, Segment* m) : father{f}, mother{m}, child(nullptr)
    {
    }

    std::string SingleChildFamily::getName() const
    {
        return name;
    }

    void SingleChildFamily::setName(const std::string& n)
    {
        name = n;
    }

    FlatlanderMale* SingleChildFamily::getFather() const
    {
        return father;
    }

    void SingleChildFamily::setFather(FlatlanderMale* f)
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

    Segment* SingleChildFamily::getMother() const
    {
        return mother;
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

    Flatlander* SingleChildFamily::getChild() const
    {
        return child;
    }

    bool SingleChildFamily::setChild(Flatlander* c)
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

    /**
     * A function to create a child,
     * it sets the child in the family and then returns the child object.
     * @return A unique pointer to the new child object
     */
    std::unique_ptr<Flatlander> SingleChildFamily::newChild()
    {
        // A dead flatlander cannot reproduce
        if (!father->isAlive() || !mother->isAlive())
        {
            throw std::logic_error("A dead Flatlander cannot reproduce");
        }

        // less than two parents
        if (father == nullptr || mother == nullptr)
        {
            throw std::logic_error("Single parent cannot produce child");
        }
        // if child exists
        if (child != nullptr)
        {
            throw std::logic_error("Cannot have more than one child");
        }

        auto res = Flatlander::newFlatlander(*mother, *father);
        child = res.get();

        return res;
    }

    void SingleChildFamily::present() const
    {
        std::cout << std::endl;
        std::cout << "======= Family details =======" << std::endl;
        presentMember("Father", father);
        presentMember("Mother", mother);
        presentMember("Child", child);
    }

    /**
     * Creates a new flatlander from two parents while respecting the generation rules
     * @param mother: Always a segment
     * @param father: Could be an isosceles or a polygone but never a segment
     * @return A unique pointer to a new flatlander.
     */
    std::unique_ptr<Flatlander> Flatlander::newFlatlander(const Segment& mother, const FlatlanderMale& father)
    {
        const std::string name = mother.getName() + "-" + father.getName(); // compound name from parents
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

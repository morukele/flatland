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
        : Flatlander(n, clamp(s, MAX_SIZE, MIN_SIZE), c)
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
    Isocele::Isocele(const std::string& n, int s, Colour c) : FlatlanderMale(n, clamp(s, MAX_SIZE, MIN_SIZE), c)
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
    Polygon::Polygon(const std::string& n, int s, Colour c, int sides) :
        FlatlanderMale(n, clamp(s, MAX_SIZE, MIN_SIZE), c),
        nbSides{clamp(sides, MAX_NB_SIZE, MIN_NB_SIZE)}
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
    Family::Family(FlatlanderMale* f, Segment* m) : father{f}, mother{m}
    {
    }

    std::string Family::getName() const
    {
        return name;
    }

    void Family::setName(const std::string& n)
    {
        name = n;
    }

    FlatlanderMale* Family::getFather() const
    {
        return father;
    }

    void Family::setFather(FlatlanderMale* f)
    {
        // check for null input passed in
        if (f != nullptr && father == nullptr)
        {
            father = f;
        }
        else
        {
            std::cout << "Father is already set or null input passed in" << std::endl;
        }
    }

    Segment* Family::getMother() const
    {
        return mother;
    }

    void Family::setMother(Segment* m)
    {
        if (m != nullptr && mother == nullptr)
        {
            mother = m;
        }
        else
        {
            std::cout << "Mother is already se or null input passed int" << std::endl;
        }
    }

    Flatlander* Family::findChild(const std::string& n) const
    {
        for (const auto& c : children)
        {
            if (c->getName() == n)
            {
                return c;
            }
        }

        // Could not find a child with the name
        return nullptr;
    }

    const std::vector<Flatlander*>& Family::getChildren() const
    {
        return children;
    }

    bool Family::adoptChild(Flatlander* c)
    {
        if (c == nullptr)
        {
            std::cout << "Family::adoptChild() called with null child" << std::endl;
            return false;
        }

        for (const auto& child : children)
        {
            if (child == c)
            {
                std::cout << "Child already adopted to this family" << std::endl;
                return false;
            }
        }

        children.push_back(c);
        return true;
    }

    /**
     * A function to create a child,
     * it sets the child in the family and then returns the child object.
     * @return A unique pointer to the new child object
     */
    Flatlander* Family::newChild()
    {
        // less than two parents
        if (father == nullptr || mother == nullptr)
        {
            throw std::logic_error("Single parent cannot produce child");
        }

        // A dead flatlander cannot reproduce
        if (!father->isAlive() || !mother->isAlive())
        {
            throw std::logic_error("A dead Flatlander cannot reproduce");
        }
        children.reserve(children.size() + 1); // Reserving capacity for one more child object
        auto res = Flatlander::newFlatlander(*mother, *father);
        children.push_back(res);

        return res;
    }

    size_t Family::getChildCount() const
    {
        return children.size();
    }

    void Family::present() const
    {
        std::cout << std::endl;
        std::cout << "======= Family details =======" << std::endl;
        presentMember("Father", father);
        presentMember("Mother", mother);
        std::cout << "Number of children: " << getChildCount() << std::endl;
        for (const auto& c : children)
        {
            presentMember("Child", c);
        }
    }

    /**
     * Creates a new flatlander from two parents while respecting the generation rules
     * @param mother: Always a segment
     * @param father: Could be an isosceles or a polygone but never a segment
     * @return A raw pointer to a new flatlander.
     */
    Flatlander* Flatlander::newFlatlander(const Segment& mother, const FlatlanderMale& father)
    {
        // compound name from parents with a random number attached
        const std::string name = mother.getName() + "-" + father.getName() + std::to_string(randInt(1, 1'000'000));
        int size = clamp((mother.getSize() + father.getSize()) / 2, MAX_SIZE, MIN_SIZE);
        Colour colour = randInt(0, 1) == 0 ? mother.getColour() : father.getColour();

        // 50% female -> Segment
        if (randInt(0, 1) == 0)
        {
            return new Segment(name, size, colour);
        }

        // Male depending on the father type
        if (dynamic_cast<const Isocele*>(&father))
        {
            if (randInt(0, 1) == 0)
            {
                return new Isocele(name, size, colour); // should be an isoceles triangle
            }

            return new Polygon(name, size, colour, 4); // should be a square
        }

        // Father is Polygon -> child is a polygon
        return new Polygon(name, size, colour, randInt(MIN_NB_SIZE, MAX_NB_SIZE));
    }
} // namespace flatland

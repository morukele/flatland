#include "flatlandien.hpp"
#include <iostream>
#include <stdexcept>

namespace flatland
{
    namespace
    {
        // An anonymous namespace for function that are private to the flatlander class
        /**
         *  A function to check if a Flatlander is registered on not into a registry.
         * @param registry a register of Flatlanders
         * @param f a Flatlander to check for in the registry
         * @return a bool if the Flatlander is in the registry or not
         */
        bool isRegistered(const std::vector<const Flatlander*>& registry, const Flatlander* f)
        {
            for (const auto element : registry)
            {
                if (element == f)
                {
                    return true;
                }
            }
            return false;
        }

        /**
         * A function to unregister a Flatlander from the registry.
         * @param registry a register of Flatlanders
         * @param f a Flatlander to check for in the registry
         * @return a bool if the Flatlander has been unregistered or not
         */
        bool unregister(std::vector<const Flatlander*>& registry, const Flatlander* f)
        {
            for (std::size_t i{0}; i < registry.size(); ++i)
            {
                if (registry[i] == f)
                {
                    registry.erase(registry.begin() + i);
                    return true;
                }
            }
            return false;
        }
    }

    /**
     * Flatlander Implementation
     */
    Flatlander::Flatlander() : name{}, size{0}, colour{}
    {
    }

    Flatlander::Flatlander(const std::string& n, const int s, const Colour c)
        : name{n}, size{s}, colour{c}
    {
        if (s < MIN_SIZE || s > MAX_SIZE)
        {
            throw invalid_intarg(s, MIN_SIZE, MAX_SIZE, "Flatlander size");
        }
    }

    std::string Flatlander::getName() const { return name; }
    void Flatlander::setName(const std::string& n) { name = n; }
    int Flatlander::getSize() const { return size; }

    void Flatlander::setSize(const int s)
    {
        if (s < MIN_SIZE || s > MAX_SIZE)
        {
            throw invalid_intarg(s, MIN_SIZE, MAX_SIZE, "Flatlander size");
        }
        size = s;
    }

    Colour Flatlander::getColour() const { return colour; }
    void Flatlander::kill() { alive = false; }
    bool Flatlander::isAlive() const { return alive; }

    std::ostream& operator<<(std::ostream& os, const Flatlander& f)
    {
        f.print(os);
        return os;
    }

    void Flatlander::print(std::ostream& os) const
    {
        os << getName() << " (size " << getSize() << ", "
            << (isAlive() ? "alive" : "dead")
            << ", " << convertColourEnumToText(getColour())
            << ")";
    }

    void Flatlander::present() const { std::cout << *this << std::endl; }

    /**
     * Segment Implementation
     */
    Segment::Segment(const std::string& n, const int s, const Colour c)
        : Flatlander(n, s, c)
    {
        if (s < MIN_SIZE || s > MAX_SIZE)
        {
            throw invalid_intarg(s, MIN_SIZE, MAX_SIZE, "Segment size");
        }
        ++numberOfSegments;
    }

    void Segment::print(std::ostream& os) const
    {
        os << "[Segment] -> ";
        Flatlander::print(os);
    }

    /**
     * Isocele Implementation
     */
    Isocele::Isocele(const std::string& n, int s, Colour c) : FlatlanderMale(n, s, c)
    {
        if (s < MIN_SIZE || s > MAX_SIZE)
        {
            throw invalid_intarg(s, MIN_SIZE, MAX_SIZE, "Isocele size");
        }
        peakAngle = randInt(MIN_ANGLE, MAX_ANGLE);
        ++numberOfIsocele;
    }

    int Isocele::getPeakAngle() const { return peakAngle; }

    void Isocele::print(std::ostream& os) const
    {
        os << "[Isocele, peak angle " << getPeakAngle() << "] -> ";
        Flatlander::print(os);
    }

    /**
     * Polygon Implementation
     */
    Polygon::Polygon(const std::string& n, int s, Colour c, int sides) :
        FlatlanderMale(n, s, c),
        nbSides{sides}
    {
        if (s < MIN_SIZE || s > MAX_SIZE)
        {
            throw invalid_intarg(s, MIN_SIZE, MAX_SIZE, "Polygon size");
        }
        if (sides < MIN_NB_SIZE || sides > MAX_NB_SIZE)
        {
            throw invalid_intarg(sides, MIN_NB_SIZE, MAX_NB_SIZE, "Polygon sides");
        }
        ++numberOfPolygon;
    }

    int Polygon::getNbSides() const { return nbSides; }

    void Polygon::print(std::ostream& os) const
    {
        os << "[Polygon, " << getNbSides() << " sides] -> ";
        Flatlander::print(os);
    }

    /**
     * Free comparison operators for Flatlanders
     * 
     * Rules:
     * - A segment cannot be compared to a FlatlanderMale (Isocele or Polygon)
     * - All segments are equal for now
     * - An Isocele is inferior to a polygon
     * - The comparison between Isocele is based entirely on the peak angle
     * - The comparison between polygon is based entirely on the number of sides.
     */

    bool operator<(const Flatlander& lhs, const Flatlander& rhs)
    {
        // Segment comparisons
        if (const auto lhsSeg = dynamic_cast<const Segment*>(&lhs))
        {
            if (const auto rhsSeg = dynamic_cast<const Segment*>(&rhs))
            {
                return false; // All segments are equal
            }
            throw std::invalid_argument("Cannot compare a Segment with another type");
        }

        // Isocele comparisons
        if (const auto lhsIso = dynamic_cast<const Isocele*>(&lhs))
        {
            if (const auto rhsIso = dynamic_cast<const Isocele*>(&rhs))
            {
                return lhsIso->getPeakAngle() < rhsIso->getPeakAngle();
            }
            if (dynamic_cast<const Polygon*>(&rhs))
            {
                // An Isocele is always inferior to a Polygon
                return true;
            }
            throw std::invalid_argument("Can only compare an Isocele to itself or a Polygon");
        }

        // Polygon comparisons
        if (const auto lhsPoly = dynamic_cast<const Polygon*>(&lhs))
        {
            if (dynamic_cast<const Isocele*>(&rhs))
            {
                // A polygon is always superior to an Isocele
                return false;
            }
            if (const auto rhsPoly = dynamic_cast<const Polygon*>(&rhs))
            {
                return lhsPoly->getNbSides() < rhsPoly->getNbSides();
            }
            throw std::invalid_argument("Can only compare a Polygon to itself or an Isocele");
        }

        throw std::invalid_argument("Cannot compare unknown Flatlander types");
    }

    bool operator>(const Flatlander& lhs, const Flatlander& rhs)
    {
        // Segment comparisons
        if (const auto lhsSeg = dynamic_cast<const Segment*>(&lhs))
        {
            if (const auto rhsSeg = dynamic_cast<const Segment*>(&rhs))
            {
                return false; // All segments are equal
            }
            throw std::invalid_argument("Cannot compare a Segment with another type");
        }

        // Isocele comparisons
        if (const auto lhsIso = dynamic_cast<const Isocele*>(&lhs))
        {
            if (const auto rhsIso = dynamic_cast<const Isocele*>(&rhs))
            {
                return lhsIso->getPeakAngle() > rhsIso->getPeakAngle();
            }
            if (dynamic_cast<const Polygon*>(&rhs))
            {
                // An Isocele is always inferior to a Polygon
                return false;
            }
            throw std::invalid_argument("Can only compare an Isocele to itself or a Polygon");
        }

        // Polygon comparisons
        if (const auto lhsPoly = dynamic_cast<const Polygon*>(&lhs))
        {
            if (dynamic_cast<const Isocele*>(&rhs))
            {
                // A polygon is always superior to an Isocele
                return true;
            }
            if (const auto rhsPoly = dynamic_cast<const Polygon*>(&rhs))
            {
                return lhsPoly->getNbSides() > rhsPoly->getNbSides();
            }
            throw std::invalid_argument("Can only compare a Polygon to itself or an Isocele");
        }

        throw std::invalid_argument("Cannot compare unknown Flatlander types");
    }

    bool operator<=(const Flatlander& lhs, const Flatlander& rhs)
    {
        // Segment comparisons
        if (const auto lhsSeg = dynamic_cast<const Segment*>(&lhs))
        {
            if (const auto rhsSeg = dynamic_cast<const Segment*>(&rhs))
            {
                return true; // All segments are equal
            }
            throw std::invalid_argument("Cannot compare a Segment with another type");
        }

        // Isocele comparisons
        if (const auto lhsIso = dynamic_cast<const Isocele*>(&lhs))
        {
            if (const auto rhsIso = dynamic_cast<const Isocele*>(&rhs))
            {
                return lhsIso->getPeakAngle() <= rhsIso->getPeakAngle();
            }
            if (dynamic_cast<const Polygon*>(&rhs))
            {
                // An Isocele is always inferior to a Polygon
                return true;
            }
            throw std::invalid_argument("Can only compare an Isocele to itself or a Polygon");
        }

        // Polygon comparisons
        if (const auto lhsPoly = dynamic_cast<const Polygon*>(&lhs))
        {
            if (dynamic_cast<const Isocele*>(&rhs))
            {
                // A polygon is always superior to an Isocele
                return false;
            }
            if (const auto rhsPoly = dynamic_cast<const Polygon*>(&rhs))
            {
                return lhsPoly->getNbSides() <= rhsPoly->getNbSides();
            }
            throw std::invalid_argument("Can only compare a Polygon to itself or an Isocele");
        }

        throw std::invalid_argument("Cannot compare unknown Flatlander types");
    }

    bool operator>=(const Flatlander& lhs, const Flatlander& rhs)
    {
        // Segment comparisons
        if (const auto lhsSeg = dynamic_cast<const Segment*>(&lhs))
        {
            if (const auto rhsSeg = dynamic_cast<const Segment*>(&rhs))
            {
                return true; // All segments are equal
            }
            throw std::invalid_argument("Cannot compare a Segment with another type");
        }

        // Isocele comparisons
        if (const auto lhsIso = dynamic_cast<const Isocele*>(&lhs))
        {
            if (const auto rhsIso = dynamic_cast<const Isocele*>(&rhs))
            {
                return lhsIso->getPeakAngle() >= rhsIso->getPeakAngle();
            }
            if (dynamic_cast<const Polygon*>(&rhs))
            {
                // An Isocele is always inferior to a Polygon
                return false;
            }
            throw std::invalid_argument("Can only compare an Isocele to itself or a Polygon");
        }

        // Polygon comparisons
        if (const auto lhsPoly = dynamic_cast<const Polygon*>(&lhs))
        {
            if (dynamic_cast<const Isocele*>(&rhs))
            {
                // A polygon is always superior to an Isocele
                return true;
            }
            if (const auto rhsPoly = dynamic_cast<const Polygon*>(&rhs))
            {
                return lhsPoly->getNbSides() >= rhsPoly->getNbSides();
            }
            throw std::invalid_argument("Can only compare a Polygon to itself or an Isocele");
        }

        throw std::invalid_argument("Cannot compare unknown Flatlander types");
    }

    bool operator==(const Flatlander& lhs, const Flatlander& rhs)
    {
        // Segment comparisons
        if (const auto lhsSeg = dynamic_cast<const Segment*>(&lhs))
        {
            if (const auto rhsSeg = dynamic_cast<const Segment*>(&rhs))
            {
                return true; // All segments are equal
            }
            throw std::invalid_argument("Cannot compare a Segment with another type");
        }

        // Isocele comparisons
        if (const auto lhsIso = dynamic_cast<const Isocele*>(&lhs))
        {
            if (const auto rhsIso = dynamic_cast<const Isocele*>(&rhs))
            {
                return lhsIso->getPeakAngle() == rhsIso->getPeakAngle();
            }
            if (dynamic_cast<const Polygon*>(&rhs))
            {
                // An Isocele is never equal to a Polygon
                return false;
            }
            throw std::invalid_argument("Can only compare an Isocele to itself or a Polygon");
        }

        // Polygon comparisons
        if (const auto lhsPoly = dynamic_cast<const Polygon*>(&lhs))
        {
            if (dynamic_cast<const Isocele*>(&rhs))
            {
                // A polygon is never equal to an Isocele
                return false;
            }
            if (const auto rhsPoly = dynamic_cast<const Polygon*>(&rhs))
            {
                return lhsPoly->getNbSides() == rhsPoly->getNbSides();
            }
            throw std::invalid_argument("Can only compare a Polygon to itself or an Isocele");
        }

        throw std::invalid_argument("Cannot compare unknown Flatlander types");
    }

    bool operator!=(const Flatlander& lhs, const Flatlander& rhs)
    {
        // Segment comparisons
        if (const auto lhsSeg = dynamic_cast<const Segment*>(&lhs))
        {
            if (const auto rhsSeg = dynamic_cast<const Segment*>(&rhs))
            {
                return false; // All segments are equal
            }
            throw std::invalid_argument("Cannot compare a Segment with another type");
        }

        // Isocele comparisons
        if (const auto lhsIso = dynamic_cast<const Isocele*>(&lhs))
        {
            if (const auto rhsIso = dynamic_cast<const Isocele*>(&rhs))
            {
                return lhsIso->getPeakAngle() != rhsIso->getPeakAngle();
            }
            if (dynamic_cast<const Polygon*>(&rhs))
            {
                // An Isocele is never equal to a Polygon
                return true;
            }
            throw std::invalid_argument("Can only compare an Isocele to itself or a Polygon");
        }

        // Polygon comparisons
        if (const auto lhsPoly = dynamic_cast<const Polygon*>(&lhs))
        {
            if (dynamic_cast<const Isocele*>(&rhs))
            {
                // A polygon is never equal to an Isocele
                return true;
            }
            if (const auto rhsPoly = dynamic_cast<const Polygon*>(&rhs))
            {
                return lhsPoly->getNbSides() != rhsPoly->getNbSides();
            }
            throw std::invalid_argument("Can only compare a Polygon to itself or an Isocele");
        }

        throw std::invalid_argument("Cannot compare unknown Flatlander types");
    }

    /**
     * Robot Surgeon Implementation
     */
    RobotSurgeon::RobotSurgeon(const std::string& n) : name{n}
    {
    }

    std::string RobotSurgeon::getName() const { return name; }
    void RobotSurgeon::modifyColour(Flatlander& flatlander, const Colour c) { flatlander.colour = c; }

    void RobotSurgeon::modifyPeakAngle(Isocele& i, const int a)
    {
        if (a < MIN_ANGLE || a > MAX_ANGLE)
        {
            throw invalid_intarg(a, MIN_ANGLE, MAX_ANGLE, "Isocele peak angle");
        }
        i.peakAngle = a;
    }

    void RobotSurgeon::modifyNbSides(Polygon& p, const int n)
    {
        if (n < MIN_NB_SIZE || n > MAX_NB_SIZE)
        {
            throw invalid_intarg(n, MIN_NB_SIZE, MAX_NB_SIZE, "Polygon sides");
        }
        p.nbSides = n;
    }

    void printStatistics()
    {
        std::cout << "Number of Segments -> " << numberOfSegments << std::endl;
        std::cout << "Number of Isocele -> " << numberOfIsocele << std::endl;
        std::cout << "Number of Polygon -> " << numberOfPolygon << std::endl;
    }

    /**
     * Static vectors to hold the information about who is a parent and who is a child
     */
    std::vector<const Flatlander*> Family::registeredParents;
    std::vector<const Flatlander*> Family::registeredChildren;

    /**
     * The public way to create a Family now
     */
    Family Family::newFamily(FlatlanderMale* f, Segment* m)
    {
        if (f != nullptr && isRegistered(registeredParents, f))
        {
            throw std::logic_error("Polygamy is not allowed: father already in a family");
        }
        if (m != nullptr && isRegistered(registeredChildren, m))
        {
            throw std::logic_error("Polygamy is not allowed: mother already in a family");
        }

        // Reserve for the two parents
        registeredParents.reserve(registeredParents.size() + 2);

        if (f != nullptr)
        {
            registeredParents.push_back(f);
        }
        if (m != nullptr)
        {
            registeredParents.push_back(m);
        }

        return Family(f, m);
    }

    /**
     * Family Implementation
     */
    Family::Family(FlatlanderMale* f, Segment* m) : father{f}, mother{m}
    {
    }

    Family::~Family()
    {
        unregister(registeredParents, father);
        unregister(registeredParents, mother);

        for (const auto child : children)
        {
            unregister(registeredChildren, child);
        }
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
            std::cout << "Mother is already set or null input passed int" << std::endl;
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

        // Checks if child is already in the family or member of another family
        if (isRegistered(registeredChildren, c))
        {
            std::cout << "Family::adoptChild() called with registered child" << std::endl;
            return false;
        }

        // Reserve first : if one of these allocations throws, neither vector has changed
        children.reserve(children.size() + 1);
        registeredChildren.reserve(registeredChildren.size() + 1);

        children.push_back(c);
        registeredChildren.push_back(c);
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

        // Reserving capacity for one more child object
        children.reserve(children.size() + 1);
        registeredChildren.reserve(registeredChildren.size() + 1);

        auto res = Flatlander::newFlatlander(*mother, *father);
        children.push_back(res);
        registeredChildren.push_back(res);

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
        int size = (mother.getSize() + father.getSize()) / 2;
        if (size < MIN_SIZE || size > MAX_SIZE)
        {
            throw invalid_intarg(size, MIN_SIZE, MAX_SIZE, "Computed child size");
        }
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

    /**
     * A function that finds all flatlanders in a family that meets a given predicate function.
     * @param predicate The predicate to compare the flatlander against
     * @param v1 The first variable in the predicate
     * @param v2 The second variable in the predicate
     * @return A vector of results that match the predicate or an empty vector if nothing matches
     */
    std::vector<Flatlander*> Family::findAll(Predicate* predicate, int v1, int v2) const
    {
        std::vector<Flatlander*> res;

        if (predicate == nullptr)
        {
            return res; // predicate is empty, so nothing matches
        }

        // Since we support single parent family, father or mother could be null
        if (father != nullptr && predicate(*father, v1, v2))
        {
            res.push_back(father);
        }
        if (mother != nullptr && predicate(*mother, v1, v2))
        {
            res.push_back(mother);
        }

        for (const auto& c : children)
        {
            if (c != nullptr && predicate(*c, v1, v2))
            {
                res.push_back(c);
            }
        }

        return res;
    }

    Object::Object(std::string id, const bool isSharable) : id{id}, isSharable{isSharable}
    {
    }

    Object::~Object() = default;

    std::string Object::getId() const { return id; }
    bool Object::getIsSharable() const { return isSharable; }

    Person::Person(std::string name) : name{name}
    {
    }

    bool Person::give(Person& receiver)
    {
        // check if giver has object to give
        // check if receiver can receive object
        if (object == nullptr || receiver.object != nullptr)
        {
            return false;
        }
        receiver.object = std::move(object);
        return true;
    }

    bool Person::borrow(std::unique_ptr<Object> obj)
    {
        if (object != nullptr || obj == nullptr || !obj->getIsSharable())
        {
            return false;
        }
        object = std::move(obj);
        return true;
    }

    void Person::specifyObject()
    {
        if (object == nullptr)
        {
            std::cout << name << " has no object" << std::endl;
            return;
        }
        std::cout << name << " has object " << object->getId() << std::endl;
    }
} // namespace flatland

#pragma once

#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include "util.hpp"

namespace flatland
{
    // ----------------------------------------------------------
    // Constants and global counters
    // ----------------------------------------------------------
    constexpr int MIN_SIZE = 5;
    constexpr int MAX_SIZE = 100;
    constexpr int MIN_ANGLE = 20;
    constexpr int MAX_ANGLE = 60;
    constexpr int MIN_NB_SIZE = 4;
    constexpr int MAX_NB_SIZE = 20;

    // use static here so that the variables last the whole lifecycle of the program.
    inline int numberOfSegments{0};
    inline int numberOfIsocele{0};
    inline int numberOfPolygon{0};

    void printStatistics();

    // ----------------------------------------------------------
    // Forward declarations
    // ----------------------------------------------------------
    class Segment;
    class FlatlanderMale;

    // ------------------------------------------------------------------
    // Flatlander: Abstract base class for all organisms in the universe
    // ------------------------------------------------------------------
    class Flatlander
    {

    public:
        friend class RobotSurgeon; // Gives RobotSurgeon access to private members

        Flatlander();
        Flatlander(const std::string &n, int s, Colour c);
        virtual ~Flatlander() = default;

        std::string getName() const;
        void setName(const std::string &n);
        int getSize() const;
        void setSize(int s);
        Colour getColour() const;

        void kill();
        bool isAlive() const;

        // Prints the Flatlander to any stream.
        // Prints the common members of Flatlanders
        virtual void print(std::ostream &os) const = 0;

        void present() const; // Abstract present in Flatlander

        static Flatlander *newFlatlander(const Segment &mother, const FlatlanderMale &father);

    private:
        std::string name{};
        int size{0};
        Colour colour{};
        bool alive{true}; // A newly constructed flatlander will always be alive
    };

    //--------------------------------------------------------
    // Operator overload for displaying the information of
    // the Flatlander object.
    //--------------------------------------------------------
    std::ostream &operator<<(std::ostream &os, const Flatlander &f);

    // ------------------------------------------------------
    // Father: an abstract representation of an adult male Flatlander (Polygone or Isocele)
    // TODO: consider changing to a generic male class that can be applied to children
    // ------------------------------------------------------
    class FlatlanderMale : public Flatlander
    {
    protected:
        // using protected here so that only subclasses can access the data.
        // It forwards the details to flatlander
        FlatlanderMale(const std::string &n, int s, Colour c) : Flatlander(n, s, c) {};
    };

    // -----------------------------------------------------
    // Segment: a female Flatlander
    // -----------------------------------------------------
    class Segment : public Flatlander
    {
    public:
        friend class RobotSurgeon;
        Segment(const std::string &n, int s, Colour c);
        void print(std::ostream &os) const override;

    };

    // ----------------------------------------------------
    // Isocele: A male Flatlander
    // ----------------------------------------------------
    class Isocele : public FlatlanderMale
    {
    public:
        friend class RobotSurgeon;
        Isocele(const std::string &n, int s, Colour c);
        int getPeakAngle() const;
        void print(std::ostream &os) const override;

    private:
        int peakAngle{};
    };

    // --------------------------------------------------
    // Polygone: A male Flatlander
    // --------------------------------------------------
    class Polygon : public FlatlanderMale
    {
    public:
        friend class RobotSurgeon;
        Polygon(const std::string &n, int s, Colour c, int sides);
        int getNbSides() const;
        void print(std::ostream &os) const override;

    private:
        int nbSides{};
    };

    // ----------------------------------------------------------
    // Free comparison operators for Flatlanders
    // ----------------------------------------------------------
    bool operator<(const Flatlander &lhs, const Flatlander &rhs);
    bool operator>(const Flatlander &lhs, const Flatlander &rhs);
    bool operator<=(const Flatlander &lhs, const Flatlander &rhs);
    bool operator>=(const Flatlander &lhs, const Flatlander &rhs);
    bool operator==(const Flatlander &lhs, const Flatlander &rhs);
    bool operator!=(const Flatlander &lhs, const Flatlander &rhs);

    // ----------------------------------------------------------
    // RobotSurgeon: A class that can manipulate the flatlanders
    // ----------------------------------------------------------
    class RobotSurgeon
    {
    public:
        explicit RobotSurgeon(const std::string &n);
        std::string getName() const; // immutable name
        static void modifyColour(Flatlander &flatlander, Colour c);
        static void modifyPeakAngle(Isocele &i, int a);
        static void modifyNbSides(Polygon &p, int n);

    private:
        const std::string name{};
    };

    //-------------------------------------------------------------
    // Predicate functions
    //-------------------------------------------------------------
    using Predicate = bool(const Flatlander &, int, int);
    using CompIfPredicate = bool(const Flatlander*, int);

    // ----------------------------------------------------------------------------
    // Family: aggregates a family, does not own any member of the family
    // ----------------------------------------------------------------------------
    // The family owns raw, non-owning pointers.
    // The creator of the family members is responsible for their life cycle management.
    // NB: The children vector is owned by the family, but the individual child Flatlander is not owned by the family.
    class Family
    {
    public:
        // Checks that the family complies with Flatlander society rules
        static Family newFamily(FlatlanderMale *f = nullptr, Segment *m = nullptr);

        ~Family();                                  // unregisters parents and children from static list
        Family(const Family &) = delete;            // prevent copies because it will put members in two different families
        Family &operator=(const Family &) = delete; // prevent assignment constructor for copy

        size_t getChildCount() const;
        std::string getName() const;
        void setName(const std::string &n);
        FlatlanderMale *getFather() const;
        void setFather(FlatlanderMale *f);
        Segment *getMother() const;
        void setMother(Segment *m);
        Flatlander *findChild(const std::string &n) const;
        const std::vector<Flatlander *> &getChildren() const;
        bool adoptChild(Flatlander *c);

        // Creates a child and links it to the family, ownership is transferred to the caller.
        // The function warns the caller about ignoring the result.
        [[nodiscard]] Flatlander *newChild();

        void present() const;

        //-------------------------------------------------------------
        // A function that finds all flatlanders in a family that meets
        // a given predicate function.
        // NB: making it a member function so that it can access private
        // members in the family class.
        //-------------------------------------------------------------
        std::vector<Flatlander *> findAll(Predicate *pred, int v1 = 0, int v2 = 0) const;

    private:
        // Private: to ensure that creation of family matches the Flatlandian society rules
        explicit Family(FlatlanderMale *f = nullptr, Segment *m = nullptr);

        // Shared by all families.
        // It is static so it lives for the entire lifetime of the program.
        // It is non owning, so it contains pointers to exisiting flatlanders.
        // The purpose is to check flatlanders that are already parents or children to someone.
        // This prevents any form of polygamy, or children with more than 2 parents
        static std::vector<const Flatlander *> registeredParents;  // fathers and mothers in a family
        static std::vector<const Flatlander *> registeredChildren; // children in a family

        std::string name{};
        FlatlanderMale *father;
        Segment *mother;
        std::vector<Flatlander *> children{};
    };

    //---------------------------------------------------------------
    // A function to present the details of a flatlandien with the label
    // --------------------------------------------------------------
    static void presentMember(const std::string &label, const Flatlander *f)
    {
        std::cout << label << ": ";
        if (f != nullptr)
        {
            f->present();
        }
        else
        {
            std::cout << "none" << std::endl;
        }
    }

    //---------------------------------------------------------------------
    // A class that overloads the () operator
    //---------------------------------------------------------------------
    class BindSecond
    {
    public:
        BindSecond(CompIfPredicate* pred, int v)
            : pred{pred}, value{v}
        {

        }

        // One argument function called by the std::find_if() function
        // This is an overload of the () operator
        bool operator()(const std::unique_ptr<Flatlander>& f) const
        {
            return pred(f.get(), value); // forwards to the two argument predicate
        }

    private:
        CompIfPredicate* pred; // The function to call
        int value; // The fixed second argument
    };

    //----------------------------------------------------------
    // Association class.
    // Template class that allows for different types of
    // associations.
    //----------------------------------------------------------
    template <typename K>
    class Association
    {
    public:
        explicit Association(const std::string& n, int& cMin) : name{n}, minimumContribution{cMin}, totalContribution{0}{}
        std::string getName() const {return name;}
        void setName(const std::string &n) {name = n;}
        int getMinimumContribution() const {return minimumContribution;}
        void setMinimumContribution(int& c) {minimumContribution = c;}
        bool registerMember(K* i, int& c)
        {
            if (c < minimumContribution)
            {
                // NB: uncomment if exception is the preferred way to go
                // throw invalid_intarg(c, minimumContribution, 1'000'000, "Minimum contribution in Association");
                std::cout << "Unable to register member because contribution -> "
                    << c << " is less than minimum contribution -> "
                    << minimumContribution << std::endl;
                return false;
            }
            totalContribution += c;
            members.push_back(i);
            return true;
        }
        int getTotalContribution() const {return totalContribution;}
        std::vector<K*> getMembers() const {return members;}

    private:
        std::string name;
        int minimumContribution;
        int totalContribution;
        std::vector<K*> members;
    };

    class Object
    {
    public:
        explicit Object(std::string  id, bool isSharable = false);
        ~Object(); // custom destructor
        std::string getId() const;
        bool getIsSharable() const;
    private:
        std::string id;
        bool isSharable;
    };

    class Person
    {
    public:
        explicit Person(std::string name);
        bool give(Person& receiver);
        bool borrow(std::unique_ptr<Object> obj);
        void specifyObject();
    private:
        std::string name;
        std::unique_ptr<Object> object; // unique pointer because the person can only own one object
    };

} // namespace flatland

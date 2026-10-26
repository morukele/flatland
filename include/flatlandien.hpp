#pragma once

#include <memory>
#include <string>

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
    inline  int numberOfSegments{0};
    inline  int numberOfIsocele{0};
    inline  int numberOfPolygon{0};

    void printStatistics();

    // ----------------------------------------------------------
    // Forward declarations
    // ----------------------------------------------------------
    class Segment;
    class Father;

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

        virtual void present() const = 0;       // Abstract present in Flatlander

        static std::unique_ptr<Flatlander> newFlatlander(const Segment &mother, const Father &father);

    private:
        std::string name{};
        int size{0};
        Colour colour{};
    };

    // ------------------------------------------------------
    // Father: an abstract representation of an adult male Flatlander (Polygone or Isocele)
    // TODO: consider changing to a generic male class that can be applied to children
    // ------------------------------------------------------
    class Father : public Flatlander
    {
    protected:
        // using protected here so that only subclasses can access the data.
        // It forwards the details to flatlander
        Father(const std::string &n, int s, Colour c) : Flatlander(n, s, c) {};
    };

    // -----------------------------------------------------
    // Segment: a female Flatlander
    // -----------------------------------------------------
    class Segment : public Flatlander
    {
    public:
        friend class RobotSurgeon;
        Segment(const std::string &n, int s, Colour c);
        void present() const override;
    };

    // ----------------------------------------------------
    // Isocele: A male Flatlander
    // ----------------------------------------------------
    class Isocele : public Father
    {
    public:
        friend class RobotSurgeon;
        Isocele(const std::string &n, int s, Colour c);
        int getPeakAngle() const;
        void present() const override;

    private:
        int peakAngle{};
    };

    // --------------------------------------------------
    // Polygone: A male Flatlander
    // --------------------------------------------------
    class Polygon : public Father
    {
    public:
        friend class RobotSurgeon;
        Polygon(const std::string &n, int s, Colour c, int sides);
        int getNbSides() const;
        void present() const override;

    private:
        int nbSides{};
    };

    // ----------------------------------------------------------
    // RobotSurgeon: A class that can manipulate the flatlanders
    // ----------------------------------------------------------
    class RobotSurgeon
    {
    public:
        explicit RobotSurgeon( const std::string& n);
        std::string getName() const; // immutable name
        static void modifyColour(Flatlander &flatlander, Colour c);
        static void modifyPeakAngle(Isocele &i, int a);
        static void modifyNbSides(Polygon &p, int n);

    private:
        const std::string name{};
    };

    // ----------------------------------------------------------------------------
    // SingleChildFamily: aggegates a family, does not own any member of the family
    // ----------------------------------------------------------------------------
    // The family owns raw, non-owning pointers.
    // The creator of the family members is responsible for their life cycle management.
    class SingleChildFamily
    {
    public:
        explicit SingleChildFamily(Father *f = nullptr, Segment *m = nullptr);

        void setFather(Father *f);
        void setMother(Segment *m);
        bool setChild(Flatlander *c);
        void divorce(const Father &initiator);
        void divorce(const Segment &initiator);

        // Creates a child and links it to the family, ownership is transferred to the caller.
        // The function warns the caller about ignoring the result.
        [[nodiscard]]std::unique_ptr<Flatlander> newChild();

        void present() const;

    private:
        Father *father;
        Segment *mother;
        Flatlander *child;
    };

} // namespace flatland

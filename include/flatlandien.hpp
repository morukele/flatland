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
    inline  int numberOfSegments{0};
    inline  int numberOfIsocele{0};
    inline  int numberOfPolygon{0};

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

        virtual void present() const = 0;       // Abstract present in Flatlander

        static Flatlander* newFlatlander(const Segment &mother, const FlatlanderMale &father);

    private:
        std::string name{};
        int size{0};
        Colour colour{};
        bool alive{true}; // A newly constructed flatlander will always be alive
    };

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
        void present() const override;
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
        void present() const override;

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
    // Family: aggregates a family, does not own any member of the family
    // ----------------------------------------------------------------------------
    // The family owns raw, non-owning pointers.
    // The creator of the family members is responsible for their life cycle management.
    // NB: The children vector is owned by the family, but the individual child Flatlander is not owned by the family.
    class Family
    {
    public:
        explicit Family(FlatlanderMale *f = nullptr, Segment *m = nullptr);

        size_t getChildCount() const;
        std::string getName() const;
        void setName(const std::string &n);
        FlatlanderMale* getFather() const;
        void setFather(FlatlanderMale *f);
        Segment *getMother() const;
        void setMother(Segment *m);
        Flatlander *findChild(const std::string& n) const;
        const std::vector<Flatlander*> &getChildren() const;
        bool adoptChild(Flatlander *c);

        // Creates a child and links it to the family, ownership is transferred to the caller.
        // The function warns the caller about ignoring the result.
        [[nodiscard]]Flatlander* newChild();

        void present() const;

    private:
        std::string name{};
        FlatlanderMale *father;
        Segment *mother;
        std::vector<Flatlander*> children{};
    };

    //---------------------------------------------------------------
    // A function to present the details of a flatlandien with the label
    // --------------------------------------------------------------
    static void presentMember(const std::string& label, const Flatlander* f)
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

} // namespace flatland

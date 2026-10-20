#pragma once

#include <memory>
#include <string>
#include <optional>

#include "util.hpp"

namespace flatland
{
    constexpr int MIN_SIZE = 5;
    constexpr int MAX_SIZE = 100;
    constexpr int MIN_ANGLE = 20;
    constexpr int MAX_ANGLE = 60;
    constexpr int MIN_NB_SIZE = 4;
    constexpr int MAX_NB_SIZE = 20;

    inline  int numberOfSegments{0}; // use static here so that the variables last the whole lifecycle of the program.
    inline  int numberOfIsocele{0};
    inline  int numberOfPolygon{0};

    void printStatistics();

    class Segment; // forward declaration
    class Father; // forward declaration

    class Flatlander
    {

    public:
        friend class RobotSurgeon;
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
        std::string name;
        int size;
        Colour colour;
    };

    class Father : public virtual Flatlander {};

    class Child : public virtual Flatlander {};

    /**
     * A class to hold the son.
     * The class sort of restrict gender transition
     */
    class Son : public Child {};

    /**
     * A class to hold the daughter
     */
    class Daughter : public Child {};


    class Segment : public Daughter
    {
    public:
        friend class RobotSurgeon;
        Segment(const std::string &n, int s, Colour c);
        void present() const override;
    };

    class Isocele : public Father, public Son
    {
    public:
        friend class RobotSurgeon;
        Isocele(const std::string &n, int s, Colour c);
        int getPeakAngle() const;
        void present() const override;

    private:
        int peakAngle{};
    };

    class Polygon : public Father, public  Son
    {
    public:
        friend class RobotSurgeon;
        Polygon(const std::string &n, int s, Colour c, int sides);
        int getNbSides() const;
        void present() const override;

    private:
        int nbSides{};
    };

    class RobotSurgeon
    {
    public:
        explicit RobotSurgeon(std::string n);
        std::string getName() const; // immutable name
        void modifyColour(Flatlander &flatlander, Colour c);
        void modifyPeakAngle(Isocele &i, int a);
        void modifyNbSides(Polygon &p, int n);

    private:
        const std::string name{};
    };

    /**
    * The family class is an aggregator and does not own any of the objects in it
    *
    * NB: Using segment for mother because no other class can be mother expect they are a segment.
    */
    class SingleChildFamily
    {
    public:
        explicit SingleChildFamily(Father *f = nullptr, Segment *m = nullptr, Child *c = nullptr); // creates an empty family
        void setFather(Father *f);
        void setMother(Segment *m);
        bool setChild(Child *c);
        void divorce(const Father &initiator);
        void divorce(const Segment &initiator);

        void present() const;

    private:
        Father *father;
        Segment *mother;
        Child *child;
    };

} // namespace flatland

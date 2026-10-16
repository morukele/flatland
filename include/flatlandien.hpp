#pragma once

#include <string>
#include <optional>

#include "util.hpp"

namespace flatland
{
    constexpr int MIN_SIZE = 5;
    constexpr int MAX_SIZE = 100;
    constexpr int MIN_ANGLE = 20;
    constexpr int MAX_ANGLE = 60;
    constexpr int MIN_nbSIZE = 4;
    constexpr int MAX_nbSIZE = 20;

    static int numberOfSegments{0}; // use static here so that the variables last the whole lifecycle of the program.
    static int numberOfIsocele{0};
    static int numberOfPolygon{0};

    void printStatistics();

    class Flatlander
    {

    public:
        friend class RobotSurgeon;
        Flatlander(std::string n, int s, Colour c);
        virtual std::string getName();
        virtual void setName(std::string n);
        virtual int getSize();
        virtual void setSize(int s);
        virtual Colour getColour();

    private:
        std::string name;
        int size;
        Colour colour;
    };

    class Father
    {
    };

    class Mother
    {
    };

    class Child
    {
    };


    class Segment : public Flatlander, public Mother
    {
    public:
        friend class RobotSurgeon;
        Segment(std::string n, int s, Colour c);
        void present();
    };

    class Isocele : public Flatlander, public Father
    {
    public:
        friend class RobotSurgeon;
        Isocele(std::string n, int s, Colour c);
        int getPeakAngle();
        void present();

    private:
        int peakAngle{};
    };

    class Polygon : public Flatlander, public Father
    {
    public:
        friend class RobotSurgeon;
        Polygon(std::string n, int s, Colour c);
        int getNbSides();
        void present();

    private:
        int nbSides;
    };

    class RobotSurgeon
    {
    public:
        explicit RobotSurgeon(std::string n);
        std::string getName(); // immutable name
        void modifyColour(Flatlander &flatlander, Colour c);
        void modifyPeakAngle(Isocele &i, int a);
        void modifyNbSides(Polygon &p, int n);

    private:
        const std::string name;
    };

    class Family
    {
    public:
        explicit Family(Father *f = nullptr, Mother *m = nullptr, Child *c = nullptr); // creates an empty family
        void setFather(Father *f);
        void setMother(Mother *m);
        void setChildren(Child *c);
        void divorce(const Father &initiator);
        void divorce(const Mother &initiator);

    private:
        Father *father;
        Mother *mother;
        Child *child;
    };

} // namespace flatland

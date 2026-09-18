#pragma once

#include <string>
#include <optional>

#include "util.hpp"

namespace flatland
{
    const static int MIN_SIZE = 5;
    const static int MAX_SIZE = 100;
    const static int MIN_ANGLE = 20;
    const static int MAX_ANGLE = 60;
    const static int MIN_nbSIZE = 4;
    const static int MAX_nbSIZE = 20;

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

    class Segment : public Flatlander
    {
    public:
        friend class RobotSurgeon;
        Segment(std::string n, int s, Colour c);
        void present();
    };

    class Isocele : public Flatlander
    {
    public:
        friend class RobotSurgeon;
        Isocele(std::string n, int s, Colour c);
        int getPeakAngle();
        void present();

    private:
        int peakAngle;
    };

    class Polygon : public Flatlander
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
        RobotSurgeon(std::string n);
        std::string getName();
        void modifyColour(Segment &s, Colour c);
        void modifyColour(Isocele &i, Colour c);
        void modifyColour(Polygon &p, Colour c);
        void modifyPeakAngle(Isocele &i, int a);
        void modifyNbSides(Polygon &p, int n);

    private:
        std::string name;
    };

} // namespace flatland

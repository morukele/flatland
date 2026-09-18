#pragma once

#include <string>

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

    class Segment
    {
    public:
        friend class RobotSurgeon;
        Segment(std::string n, int s, Colour c);
        std::string getName();
        void setName(std::string n);
        int getSize();
        void setSize(int s);
        Colour getColor();
        void present();

    private:
        std::string name;
        int size;
        Colour colour;
    };

    class Isocele
    {
    public:
        friend class RobotSurgeon;
        Isocele(std::string n, int s, Colour c);
        std::string getName();
        void setName(std::string n);
        int getSize();
        void setSize(int s);
        Colour getColour();
        int getPeakAngle();
        void present();

    private:
        std::string name;
        int size;
        Colour colour;
        int peakAngle;
    };

    class Polygon
    {
    public:
        friend class RobotSurgeon;
        Polygon(std::string n, int s, Colour c);
        std::string getName();
        void setName(std::string n);
        int getSize();
        void setSize(int s);
        Colour getColour();
        int getNbSides();
        void present();

    private:
        std::string name;
        int size;
        Colour colour;
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

#pragma once

#include <string>

#include "util.hpp"

namespace flatland
{
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

    class Polygone
    {
    public:
        friend class RobotSurgeon;
        Polygone(std::string n, int s, Colour c);
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
        void modifyColour(Polygone &p, Colour c);
        void modifyPeakAngle(Isocele &i, int a);
        void modifyNbSides(Polygone &p, int n);

    private:
        std::string name;
    };

} // namespace flatland

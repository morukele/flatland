#include <iostream>
#include <string>
#include "util.hpp"

class Segment
{
public:
    Segment(std::string n, int s, Colour c) : name{n}, size{s}, colour{c} {}
    std::string getName() { return name; }
    void setName(std::string n) { name = n; }
    int getSize() { return size; }
    void setSize(int s) { size = s; }
    Colour getColor() { return colour; }
    void present()
    {
        std::cout << "Greeting from Segment -> " << this->name << "with size -> " << this->size << std::endl;
    }

private:
    std::string name;
    int size;
    Colour colour;
};

class Isocele
{
public:
    Isocele(std::string n, int s, Colour c) : name{n}, size{s}, colour{c} {}
    std::string getName() { return name; }
    void setName(std::string n) { name = n; }
    int getSize() { return size; }
    void setSize(int s) { size = s; }
    Colour getColour() { return colour; }
    int getPeakAngle() { return peakAngle; }
    void present()
    {
        std::cout << "Greeting from Isocele -> " << this->name << "with size -> " << this->size << std::endl;
    }

private:
    std::string name;
    int size;
    Colour colour;
    int peakAngle;
};

class Polygone
{
public:
    Polygone(std::string n, int s, Colour c) : name{n}, size{s}, colour{c} {}
    std::string getName() { return name; }
    void setName(std::string n) { name = n; }
    int getSize() { return size; }
    void setSize(int s) { size = s; }
    Colour getColour() { return colour; }
    int getNbSides() { return nbSides; }
    void present()
    {
        std::cout << "Greetings from Polygone -> " << this->name << "with size -> " << this->size << std::endl;
    }

private:
    std::string name;
    int size;
    Colour colour;
    int nbSides;
};
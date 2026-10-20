#include "flatlandien.hpp"
#include <string>
#include <iostream>

using namespace flatland;

/*
 * Test function 1
 */
void Q1()
{
    std::cout << "----------------Running Test 1------------" << std::endl;

    Segment seg(std::string{"segment"}, 10, Colour::RED);
    Isocele iso(std::string{"Isocele"}, 20, Colour::BLUE);
    Polygon poly(std::string{"Polygon"}, 15, Colour::GREEN, randInt(MIN_NB_SIZE, MAX_NB_SIZE));

    seg.present();
    iso.present();
    poly.present();

    std::cout << std::endl;
}

/**
 * Test function 2
 */
void Q2()
{
    std::cout << "----------------Running Test 2------------" << std::endl;

    Segment seg(std::string{"segment"}, 10, Colour::RED);
    Isocele iso(std::string{"Isocele"}, 20, Colour::BLUE);
    Polygon poly(std::string{"Polygon"}, 15, Colour::GREEN, randInt(MIN_NB_SIZE, MAX_NB_SIZE));
    RobotSurgeon surgeon(std::string{"Dr BES"});

    seg.present();
    iso.present();
    poly.present();

    std::cout << std::endl;

    surgeon.modifyColour(poly, Colour::BLACK);
    surgeon.modifyColour(seg, Colour::WHITE);
    surgeon.modifyColour(iso, Colour::YELLOW);

    poly.setSize(200);
    iso.setSize(4);
    seg.setSize(50);

    poly.present();
    iso.present();
    seg.present();

    // Setting up the family
    SingleChildFamily family;
    family.setFather(&iso);
    family.setMother(&seg);
    auto res = family.setChild(&poly);
    auto res2 = family.setChild(&iso);

    family.present();

    std::cout << std::endl;
}

int main()
{
    Q1();
    Q2();

    printStatistics();

    return 0;
}

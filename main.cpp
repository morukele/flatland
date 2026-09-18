#include "flatlandien.hpp"
#include <string>
#include <iostream>

/*
 * Test function 1
 */
void Q1()
{
    std::cout << "----------------Running Test 1------------" << std::endl;

    flatland::Segment seg(std::string{"segment"}, 10, flatland::Colour::RED);
    flatland::Isocele iso(std::string{"Isocele"}, 20, flatland::Colour::BLUE);
    flatland::Polygone poly(std::string{"Polygone"}, 15, flatland::Colour::GREEN);

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

    flatland::Segment seg(std::string{"segment"}, 10, flatland::Colour::RED);
    flatland::Isocele iso(std::string{"Isocele"}, 20, flatland::Colour::BLUE);
    flatland::Polygone poly(std::string{"Polygone"}, 15, flatland::Colour::GREEN);
    flatland::RobotSurgeon surgeon(std::string{"Dr BES"});

    seg.present();
    iso.present();
    poly.present();

    std::cout << std::endl;

    surgeon.modifyColour(poly, flatland::Colour::BLACK);
    surgeon.modifyColour(seg, flatland::Colour::WHITE);
    surgeon.modifyColour(iso, flatland::Colour::YELLOW);

    poly.present();
    iso.present();
    poly.present();

    std::cout << std::endl;
}

int main()
{
    Q1();
    Q2();
    return 0;
}
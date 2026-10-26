#include "flatlandien.hpp"
#include <string>
#include <iostream>

using namespace flatland;

/*
 * Test function 1
 */
static void Q1()
{
    std::cout << "----------------Running Test 1------------" << std::endl;

    const Segment seg(std::string{"segment"}, 10, Colour::RED);
    const Isocele iso(std::string{"Isocele"}, 20, Colour::BLUE);
    const Polygon poly(std::string{"Polygon"}, 15, Colour::GREEN, randInt(MIN_NB_SIZE, MAX_NB_SIZE));

    seg.present();
    iso.present();
    poly.present();

    std::cout << std::endl;
}

/**
 * Test function 2
 */
static void Q2()
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
    family.setChild(&poly);

    family.present();

    std::cout << std::endl;
}

/**
 * A test function to create new flatlanders and see if the probability matches
 */
static void Q3_1()
{
    int segments{0};
    int isoceles{0};
    int polygons{0};

    std::cout << "----------------Running Test 3------------" << std::endl;
    const Segment mother(std::string{"Mother"}, 10, Colour::RED);
    const Isocele father(std::string{"Father"}, 20, Colour::BLUE);

    for (int i{}; i < 10'000; ++i)
    {
        auto res = Flatlander::newFlatlander(mother, father);
        if (dynamic_cast<Segment*>(res.get()))
        {
            segments++;
        }
        else if (dynamic_cast<Isocele*>(res.get()))
        {
            isoceles++;
        }
        else
        {
            polygons++;
        }
    }

    const auto total = segments + isoceles + polygons;

    std::cout << "Segments: " << (static_cast<double>(segments) / total) * 100 << "%" << std::endl;
    std::cout << "Isoceles: " << (static_cast<double>(isoceles) / total) * 100 << "%" << std::endl;
    std::cout << "Polygons: " << (static_cast<double>(polygons) / total) * 100 << "%" << std::endl;
}

static void Q3_2()
{
    int segments{0};
    int isoceles{0};
    int polygons{0};

    std::cout << "----------------Running Test 4------------" << std::endl;
    const Segment mother(std::string{"Mother"}, 10, Colour::RED);
    const Polygon father(std::string{"Father"}, 20, Colour::BLUE, randInt(MIN_NB_SIZE, MAX_NB_SIZE));

    for (int i{}; i < 10'000; ++i)
    {
        auto res = Flatlander::newFlatlander(mother, father);
        if (dynamic_cast<Segment*>(res.get()))
        {
            segments++;
        }
        else if (dynamic_cast<Isocele*>(res.get()))
        {
            isoceles++;
        }
        else
        {
            polygons++;
        }
    }

    const auto total = segments + isoceles + polygons;

    std::cout << "Segments: " << (static_cast<double>(segments) / total) * 100 << "%" << std::endl;
    std::cout << "Isoceles: " << (static_cast<double>(isoceles) / total) * 100 << "%" << std::endl;
    std::cout << "Polygons: " << (static_cast<double>(polygons) / total) * 100 << "%" << std::endl;
}

static void Q4_1()
{
    Segment mother(std::string{"Mother"}, 10, Colour::RED);
    Isocele father(std::string{"Father"}, 20, Colour::BLUE);

    SingleChildFamily family(&father, &mother);
    auto child = family.newChild(); // create new child

    family.present();
}

int main()
{
    Q1();
    Q2();

    printStatistics();

    Q3_1();
    Q3_2();

    Q4_1();

    return 0;
}

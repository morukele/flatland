#include "include/flatlandien.hpp"
#include <string>

// -------- Test function 1 -----------------//
void Q1()
{
    Segment seg(std::string{"segment"}, 10, Colour::RED);
    Isocele iso(std::string{"Isocele"}, 20, Colour::BLUE);
    Polygone poly(std::string{"Polygone"}, 15, Colour::GREEN);

    seg.present();
    iso.present();
    poly.present();
}

int main()
{
    Q1();
    return 0;
}
#include "flatlandien.hpp"
#include <string>
#include <iostream>
#include <sstream>

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
    Family family = Family::newFamily(&iso, &seg);
    family.adoptChild(&poly);

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

    std::cout << "----------------Running Test 3_1------------" << std::endl;
    const Segment mother(std::string{"Mother"}, 10, Colour::RED);
    const Isocele father(std::string{"Father"}, 20, Colour::BLUE);

    for (int i{}; i < 10'000; ++i)
    {
        auto res = Flatlander::newFlatlander(mother, father);
        if (dynamic_cast<Segment*>(res))
        {
            segments++;
        }
        else if (dynamic_cast<Isocele*>(res))
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

    std::cout << "----------------Running Test 3_2------------" << std::endl;
    const Segment mother(std::string{"Mother"}, 10, Colour::RED);
    const Polygon father(std::string{"Father"}, 20, Colour::BLUE, randInt(MIN_NB_SIZE, MAX_NB_SIZE));

    for (int i{}; i < 10'000; ++i)
    {
        auto res = Flatlander::newFlatlander(mother, father);
        if (dynamic_cast<Segment*>(res))
        {
            segments++;
        }
        else if (dynamic_cast<Isocele*>(res))
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

static std::pair<Segment*, FlatlanderMale*> generateFatherAndMother(
    const std::string& motherName, const std::string& fatherName)
{
    auto mother = new Segment(motherName, randInt(MIN_SIZE, MAX_SIZE), randomColour());
    FlatlanderMale* father;
    if (randInt(0, 1) == 0)
    {
        father = new Isocele(fatherName, randInt(MIN_SIZE, MAX_SIZE), randomColour());
    }
    else
    {
        father = new Polygon(fatherName, randInt(MIN_SIZE, MAX_SIZE), randomColour(),
                             randInt(MIN_NB_SIZE, MAX_NB_SIZE));
    }

    return std::make_pair(mother, father);
}

static void Q4_1()
{
    std::cout << "----------------Running Test 4_1------------" << std::endl;
    auto [mother, father] = generateFatherAndMother("Mother", "Father");

    Family family = Family::newFamily(father, mother);
    auto child = family.newChild(); // create new child

    family.present();
    std::cout << std::endl;
}

static void Q5()
{
    std::cout << "----------------Running Test 5------------" << std::endl;
    auto [mother, father] = generateFatherAndMother("Alice", "Bob");
    Family family = Family::newFamily(father, mother);

    father->present();
    mother->present();

    auto kid = family.newChild();
    kid->present();

    // Dead father cannot reproduce
    std::cout << std::endl;
    std::cout << "--- Killing father ---" << std::endl;
    auto [mother2, father2] = generateFatherAndMother("Alice", "Bob");
    Family family2 = Family::newFamily(father2, mother2);
    father2->kill();
    father2->present();
    try
    {
        auto kid2 = family2.newChild();
        std::cout << "ERROR: a dead father produced a child, it should not happen" << std::endl;
    }
    catch (const std::logic_error& e)
    {
        std::cout << "Expected error: " << e.what() << std::endl;
    }

    // Dead mother cannot reproduce
    std::cout << std::endl;
    std::cout << "--- Killing mother ---" << std::endl;
    auto [mother3, father3] = generateFatherAndMother("Alice", "Bob");
    Family family3 = Family::newFamily(father3, mother3);
    mother3->kill();
    mother3->present();

    try
    {
        auto kid3 = family3.newChild();
        std::cout << "ERROR: a dead mother produced a child, it should happen" << std::endl;
    }
    catch (const std::logic_error& e)
    {
        std::cout << "Expected error: " << e.what() << std::endl;
    }
}

static void Q6()
{
    std::cout << "----------------Running Test 6------------" << std::endl;
    auto [mother, father] = generateFatherAndMother("Alice", "Bob");

    // Owners of the children.
    std::vector<Flatlander*> owners;

    Family family = Family::newFamily(father, mother);
    family.setName("Orukele");

    std::cout << "--- Empty Family ---" << std::endl;
    father->present();

    // Several birth events
    std::cout << "\n--- Three Births ---\n";
    for (int i{}; i < 3; ++i)
    {
        owners.push_back(family.newChild());
    }
    std::cout << "child count: " << family.getChildCount() << std::endl;
    family.present();

    // Adoption of an existing Flatlander
    std::cout << "\n--- Adoption ---\n";
    Polygon orphan("Orphan", 18, randomColour(), 6);
    std::cout << std::boolalpha
        << "adopted: " << family.adoptChild(&orphan) << std::endl
        << "adopted again (duplicate): " << family.adoptChild(&orphan) << std::endl
        << "adopted nullptr: " << family.adoptChild(nullptr) << std::endl
        << "child count: " << family.getChildCount() << std::endl;
    family.present();

    // A dead parent cannot give birth
    std::cout << "\n--- Father dies ---\n";
    father->kill();
    try
    {
        auto kid = family.newChild();
        std::cout << "ERROR: Dead father produced a child, it should not happen" << std::endl;
    }
    catch (const std::logic_error& e)
    {
        std::cout << "Expected error: " << e.what() << std::endl;
    }
    std::cout << "Child count unchanged: " << family.getChildCount() << std::endl;
    family.present();
}

static void Q7()
{
    std::cout << "------------------Running Test 7-------------" << std::endl;
    auto [alice, bob] = generateFatherAndMother("Alice", "Bob");
    Polygon orphan("Orphan", 18, randomColour(), 6);

    {
        Family f1 = Family::newFamily(bob, alice);
        std::cout << f1.adoptChild(&orphan) << std::endl;
    }

    Family f2 = Family::newFamily(bob, alice);
    std::cout << f2.adoptChild(&orphan) << std::endl;
}


//------------------------------------------------------
// Predicate functions
//------------------------------------------------------

// Size between v1 and v2
bool hasSizeBetween(const Flatlander& f, int v1, int v2)
{
    return f.getSize() >= v1 && f.getSize() <= v2;
}

// Is an isocele triangle
bool isIsoceleTriangle(const Flatlander& f, int v1, int /*unused*/)
{
    const auto* i = dynamic_cast<const Isocele*>(&f);
    return i != nullptr && i->getPeakAngle() > v1;
}

// If the flatlander is alive or not
bool isDead(const Flatlander& f, int /*unused*/, int /*unused*/)
{
    return !f.isAlive();
}

auto hasSizeBetweenLambda = [](const Flatlander& f, int v1, int v2) { return f.getSize() >= v1 && f.getSize() <= v2; };
auto isIsoceleTriangleLambda = [](const Flatlander& f, int v1, int /*unused*/)
{
    const auto* i = dynamic_cast<const Isocele*>(&f);
    return i != nullptr && i->getPeakAngle() > v1;
};
auto isDeadLambda = [](const Flatlander& f, int /*unused*/, int /*unused*/) { return !f.isAlive(); };

static void Q8()
{
    Segment alice("Alice", 20, Colour::RED);
    Isocele bob("Bob", 30, Colour::BLUE);
    Family family = Family::newFamily(&bob, &alice);

    Polygon orphan("Orphan", 18, randomColour(), 6);
    family.adoptChild(&orphan);

    Flatlander* child = family.newChild();

    bob.kill();

    auto printAll = [](const std::string& title, const std::vector<Flatlander*>& v)
    {
        std::cout << title << " (" << v.size() << "):" << std::endl;
        for (int i{}; i < v.size(); ++i)
        {
            std::cout << "  ";
            v[i]->present();
        }
    };

    printAll("Size is between 10 and 25", family.findAll(hasSizeBetweenLambda, 10, 25));
    printAll("Isoceles with angle > 55°", family.findAll(isIsoceleTriangleLambda, 55));
    printAll("Dead", family.findAll(isDeadLambda));
}

static void Q9()
{
    Segment alice("Alice", 20, Colour::RED);
    Isocele bob("Bob", 30, Colour::BLUE);
    Polygon square("Square", 15, Colour::GREEN, 4);

    RobotSurgeon::modifyPeakAngle(bob, 40);

    std::cout << "--- Direct printing, on per type ---" << std::endl;
    std::cout << alice << std::endl;
    std::cout << bob << std::endl;
    std::cout << square << std::endl;

    std::cout << "--- Through a base reference (polymorphism) ---" << std::endl;
    const Flatlander& ref = bob;
    std::cout << ref << std::endl;

    std::cout << "--- Through base pointers ---" << std::endl;
    std::vector<const Flatlander*> all{&alice, &bob, &square};
    for (auto f : all)
    {
        std::cout << *f << std::endl;
    }

    std::cout << "--- Chaining and state change ---" << std::endl;
    bob.kill();
    std::cout << "After death: " << bob << " | and " << alice << std::endl;

    std::cout << "--- Family ---" << std::endl;
    Family family = Family::newFamily(&square, &alice);
    Flatlander* kid = family.newChild();
    std::cout << "Newborn: " << *kid << '\n';

    std::cout << "--- Automatic check with ostringstream ---\n";
    std::ostringstream oss;
    oss << bob;
    const std::string text = oss.str();
    std::cout << "captured: " << text << "\n";
    std::cout << std::boolalpha
        << "contains \"Isocele\": " << (text.find("Isocele") != std::string::npos) << "\n"
        << "contains \"dead\": " << (text.find("dead") != std::string::npos) << "\n";

    std::cout << "--- present() still works ---\n";
    alice.present();
}

static void Q10()
{
    Segment alice("Alice", 20, Colour::RED);
    Segment rebecca("rebecca", 30, Colour::BLUE);
    Isocele bob("Bob", 30, Colour::BLUE);
    Polygon square("Square", 15, Colour::GREEN, 4);

    std::cout << "Comparing Segment to other types" << std::endl;
    std::cout << std::boolalpha << "Alice is less than Rebecca: " << (alice < rebecca) << std::endl;
    try
    {
        auto res = alice < bob;
    }
    catch (std::exception& e)
    {
        std::cerr << "Expected error [Segment and Isocele]: " << e.what() << std::endl;
    }
    try
    {
        auto res = alice < square;
    }
    catch (std::exception& e)
    {
        std::cerr << "Expected error [Segment and Polygon]: " << e.what() << std::endl;
    }

    std::cout << "Comparing Isocele to other types" << std::endl;
    std::cout << std::boolalpha << "Bob is less than Square: " << (bob < square) << std::endl;
}

static void Q11()
{
    std::cout << std::boolalpha;

    // --- Segment comparisons ---
    Segment s1("S1", 10, Colour::RED);
    Segment s2("S2", 20, Colour::BLUE);

    std::cout << "=== Segment vs Segment ===" << std::endl;
    std::cout << "s1 < s2:  " << (s1 < s2) << " (expected: false)" << std::endl;
    std::cout << "s1 > s2:  " << (s1 > s2) << " (expected: false)" << std::endl;
    std::cout << "s1 <= s2: " << (s1 <= s2) << " (expected: true)" << std::endl;
    std::cout << "s1 >= s2: " << (s1 >= s2) << " (expected: true)" << std::endl;
    std::cout << "s1 == s2: " << (s1 == s2) << " (expected: true)" << std::endl;
    std::cout << "s1 != s2: " << (s1 != s2) << " (expected: false)" << std::endl;

    // --- Segment vs Isocele (should throw) ---
    Isocele i1("I1", 25, Colour::GREEN);
    std::cout << "\n=== Segment vs Isocele (should throw) ===" << std::endl;
    try
    {
        auto res = s1 < i1;
        std::cerr << "ERROR: Should have thrown" << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << "Expected error: " << e.what() << std::endl;
    }
    try
    {
        auto res = s1 == i1;
        std::cerr << "ERROR: Should have thrown" << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << "Expected error: " << e.what() << std::endl;
    }

    // --- Segment vs Polygon (should throw) ---
    Polygon p1("P1", 15, Colour::YELLOW, 6);
    std::cout << "\n=== Segment vs Polygon (should throw) ===" << std::endl;
    try
    {
        auto res = s1 <= p1;
        std::cerr << "ERROR: Should have thrown" << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << "Expected error: " << e.what() << std::endl;
    }

    // --- Isocele vs Isocele (same peak angle) ---
    // Note: peakAngle is random, so we use the same object to test equality
    std::cout << "\n=== Isocele vs Isocele (same object) ===" << std::endl;
    std::cout << "i1 < i1:  " << (i1 < i1) << " (expected: false)" << std::endl;
    std::cout << "i1 > i1:  " << (i1 > i1) << " (expected: false)" << std::endl;
    std::cout << "i1 <= i1: " << (i1 <= i1) << " (expected: true)" << std::endl;
    std::cout << "i1 >= i1: " << (i1 >= i1) << " (expected: true)" << std::endl;
    std::cout << "i1 == i1: " << (i1 == i1) << " (expected: true)" << std::endl;
    std::cout << "i1 != i1: " << (i1 != i1) << " (expected: false)" << std::endl;

    // --- Isocele vs Isocele (different peak angles) ---
    // We use RobotSurgeon to set specific peak angles for deterministic testing
    Isocele i2("I2", 30, Colour::RED);
    RobotSurgeon surgeon("TestSurgeon");
    surgeon.modifyPeakAngle(i1, 30);
    surgeon.modifyPeakAngle(i2, 50);

    std::cout << "\n=== Isocele vs Isocele (different peak angles) ===" << std::endl;
    std::cout << "i1 peak: " << i1.getPeakAngle() << ", i2 peak: " << i2.getPeakAngle() << std::endl;
    std::cout << "i1 < i2:  " << (i1 < i2) << " (expected: true)" << std::endl;
    std::cout << "i1 > i2:  " << (i1 > i2) << " (expected: false)" << std::endl;
    std::cout << "i1 <= i2: " << (i1 <= i2) << " (expected: true)" << std::endl;
    std::cout << "i1 >= i2: " << (i1 >= i2) << " (expected: false)" << std::endl;
    std::cout << "i1 == i2: " << (i1 == i2) << " (expected: false)" << std::endl;
    std::cout << "i1 != i2: " << (i1 != i2) << " (expected: true)" << std::endl;

    // --- Isocele vs Polygon ---
    std::cout << "\n=== Isocele vs Polygon ===" << std::endl;
    std::cout << "i1 < p1:  " << (i1 < p1) << " (expected: true - Isocele inferior to Polygon)" << std::endl;
    std::cout << "i1 > p1:  " << (i1 > p1) << " (expected: false)" << std::endl;
    std::cout << "i1 <= p1: " << (i1 <= p1) << " (expected: true)" << std::endl;
    std::cout << "i1 >= p1: " << (i1 >= p1) << " (expected: false)" << std::endl;
    std::cout << "i1 == p1: " << (i1 == p1) << " (expected: false)" << std::endl;
    std::cout << "i1 != p1: " << (i1 != p1) << " (expected: true)" << std::endl;

    // --- Isocele vs Segment (should throw) ---
    std::cout << "\n=== Isocele vs Segment (should throw) ===" << std::endl;
    try
    {
        auto res = i1 < s1;
        std::cerr << "ERROR: Should have thrown" << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << "Expected error: " << e.what() << std::endl;
    }

    // --- Polygon vs Polygon (same sides) ---
    Polygon p2("P2", 20, Colour::BLUE, 6);
    std::cout << "\n=== Polygon vs Polygon (same sides) ===" << std::endl;
    std::cout << "p1 sides: " << p1.getNbSides() << ", p2 sides: " << p2.getNbSides() << std::endl;
    std::cout << "p1 < p2:  " << (p1 < p2) << " (expected: false)" << std::endl;
    std::cout << "p1 > p2:  " << (p1 > p2) << " (expected: false)" << std::endl;
    std::cout << "p1 <= p2: " << (p1 <= p2) << " (expected: true)" << std::endl;
    std::cout << "p1 >= p2: " << (p1 >= p2) << " (expected: true)" << std::endl;
    std::cout << "p1 == p2: " << (p1 == p2) << " (expected: true)" << std::endl;
    std::cout << "p1 != p2: " << (p1 != p2) << " (expected: false)" << std::endl;

    // --- Polygon vs Polygon (different sides) ---
    Polygon p3("P3", 25, Colour::RED, 10);
    std::cout << "\n=== Polygon vs Polygon (different sides) ===" << std::endl;
    std::cout << "p1 sides: " << p1.getNbSides() << ", p3 sides: " << p3.getNbSides() << std::endl;
    std::cout << "p1 < p3:  " << (p1 < p3) << " (expected: true)" << std::endl;
    std::cout << "p1 > p3:  " << (p1 > p3) << " (expected: false)" << std::endl;
    std::cout << "p1 <= p3: " << (p1 <= p3) << " (expected: true)" << std::endl;
    std::cout << "p1 >= p3: " << (p1 >= p3) << " (expected: false)" << std::endl;
    std::cout << "p1 == p3: " << (p1 == p3) << " (expected: false)" << std::endl;
    std::cout << "p1 != p3: " << (p1 != p3) << " (expected: true)" << std::endl;

    // --- Polygon vs Isocele ---
    std::cout << "\n=== Polygon vs Isocele ===" << std::endl;
    std::cout << "p1 < i1:  " << (p1 < i1) << " (expected: false - Polygon superior to Isocele)" << std::endl;
    std::cout << "p1 > i1:  " << (p1 > i1) << " (expected: true)" << std::endl;
    std::cout << "p1 <= i1: " << (p1 <= i1) << " (expected: false)" << std::endl;
    std::cout << "p1 >= i1: " << (p1 >= i1) << " (expected: true)" << std::endl;
    std::cout << "p1 == i1: " << (p1 == i1) << " (expected: false)" << std::endl;
    std::cout << "p1 != i1: " << (p1 != i1) << " (expected: true)" << std::endl;

    // --- Polygon vs Segment (should throw) ---
    std::cout << "\n=== Polygon vs Segment (should throw) ===" << std::endl;
    try
    {
        auto res = p1 >= s1;
        std::cerr << "ERROR: Should have thrown" << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << "Expected error: " << e.what() << std::endl;
    }
}

static std::vector<std::unique_ptr<Flatlander>> createMaleFlatlanders(const int n)
{
    std::vector<std::unique_ptr<Flatlander>> v; // using unique pointer because abstract

    for (int i{0}; i < n; i++)
    {
        constexpr int upper = 50;
        // 50% Polygone
        if (randInt(0, 1) == 0)
        {
            const auto name = "p" + std::to_string(i);
            auto p = std::make_unique<Polygon>(name, randInt(0, upper), randomColour(), randInt(0, upper));
            v.push_back(std::move(p));
        }
        else
        {
            // 50% Isocele
            const auto name = "i" + std::to_string(i);
            auto p = std::make_unique<Isocele>(name, randInt(0, upper), randomColour());
            v.push_back(std::move(p));
        }
    }
    return v;
}

bool comp(const std::unique_ptr<Flatlander>& f1, const std::unique_ptr<Flatlander>& f2)
{
    return f1 > f2;
}

static void Q12()
{
    auto flatlanders = createMaleFlatlanders(10);

    std::cout << "====== Before Sort ======" << std::endl;
    for (const auto& f : flatlanders)
    {
        f->present();
    }

    std::cout << "\n";

    std::cout << "====== After Sort ======" << std::endl;
    std::sort(flatlanders.begin(), flatlanders.end(), comp);
    for (const auto& f : flatlanders)
    {
        f->present();
    }
}

int main()
{
    Q12();
}

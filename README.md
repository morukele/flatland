# Flatland

Flatland is a C++23 simulation inspired by Edwin A. Abbott's _Flatland_. It demonstrates object-oriented C++ concepts including inheritance, polymorphism, encapsulation, friend classes, namespaces, enum classes, custom exceptions, operator overloading, smart pointers, predicate functions, and CMake builds.

## Features

- **Inheritance hierarchy**: `Flatlander` abstract base class with `FlatlanderMale`, `Segment`, `Isocele`, and `Polygon` derived classes.
- **Family system**: `Family` class that manages parent-child relationships with registry-based validation to enforce monogamy and unique parentage.
- **Shape properties**: Names, sizes, and colours with getters and setters, validated against configurable limits.
- **RobotSurgeon**: A friend class that can modify protected shape properties (colour, peak angle, number of sides).
- **Lifecycle management**: `kill()` and `isAlive()` methods for tracking flatlander state.
- **Comparison operators**: Full set of comparison operators (`<`, `>`, `<=`, `>=`, `==`, `!=`) for flatlanders.
- **Stream output**: Overloaded `operator<<` for convenient console and stream output.
- **Custom exceptions**: `invalid_intarg` exception with min/max range information for value validation.
- **Colour utilities**: `Colour` enum with text conversion and random colour generation.
- **Random utilities**: `randInt()` and `randomColour()` helpers for generating random values.
- **Predicate functions**: `BindSecond` functor and predicate-based searching within families.
- **Runtime statistics**: Tracking of constructed shape counts across the program lifetime.
- **Demonstration program**: `main.cpp` contains extensive test scenarios exercising all features.

## Requirements

- A C++ compiler with C++23 support.
- CMake 4.1 or newer.
- Make, Ninja, or another generator supported by CMake.
- macOS, Linux, or another platform supported by the selected compiler and CMake.

The current project configuration requests the C++23 standard in `CMakeLists.txt`.

## Building

From the project root:

```sh
cmake -S . -B build
cmake --build build
```

The executable is written to the build directory:

```sh
./build/flatland
```

On a multi-configuration generator, such as Visual Studio, specify a configuration when building and running:

```sh
cmake --build build --config Release
./build/Release/flatland
```

## Example Output

The exact formatting may change as the demonstration evolves, but a run currently presents the shapes, applies updates through `RobotSurgeon`, and prints statistics similar to:

```text
Greeting from Segment -> segmentwith size -> 10 with colour -> RED
Greeting from Isocele -> Isocelewith size -> 20 with colour -> BLUE
Greetings from Polygon -> Polygon with size -> 15 with colour -> GREEN
...
Number of Segments -> 2
Number of Isocele -> 2
Number of Polygon -> 2
```

The statistics are cumulative for the lifetime of the process. The sample creates one instance of each shape in `Q1()` and another instance of each shape in `Q2()`.

## Project Structure

```text
.
├── CMakeLists.txt
├── main.cpp
├── include/
│   ├── flatlandien.hpp
│   └── util.hpp
└── src/
    └── flatlandien.cpp
```

- `CMakeLists.txt`: project and executable configuration.
- `main.cpp`: demonstration program with multiple test scenarios.
- `include/flatlandien.hpp`: all class declarations (Flatlander hierarchy, Family, RobotSurgeon, operators, predicates).
- `include/util.hpp`: `Colour` enum, colour conversion, `clamp()`, random utilities, and `invalid_intarg` exception.
- `src/flatlandien.cpp`: class method implementations and statistics.

Generated build files belong in `build/` and should not be committed to source control. Common build artifacts are listed in `.gitignore`.

## Namespace

The public API is contained in the `flatland` namespace:

```cpp
#include "flatlandien.hpp"

int main()
{
    flatland::Segment segment{
        "side",
        10,
        flatland::Colour::RED};

    segment.present();
    flatland::printStatistics();
}
```

## Core API

### Inheritance Hierarchy

The class hierarchy models the organisms of Flatland:

```
Flatlander (abstract base)
├── Segment (female)
└── FlatlanderMale (abstract male base)
    ├── Isocele (male, isosceles triangle)
    └── Polygon (male, multi-sided)
```

#### `Flatlander`

Abstract base class for all flatlanders. Provides:

- A constructor accepting a name, size, and colour.
- `getName()` and `setName()`.
- `getSize()` and `setSize()`.
- `getColour()`.
- `kill()` and `isAlive()` for lifecycle management.
- `present()` for console output.
- `print(std::ostream&)` — pure virtual, implemented by each derived class.
- `newFlatlander(const Segment&, const FlatlanderMale&)` — static factory that creates a child of the given parents.

#### `FlatlanderMale`

Abstract intermediate base for male flatlanders (`Isocele` and `Polygon`). Inherits from `Flatlander` and provides no additional public members beyond the protected constructor.

#### `Segment`

Represents a female flatlander. Inherits from `Flatlander`. No additional members beyond the base class.

#### `Isocele`

Represents a male flatlander shaped as an isosceles triangle. Inherits from `FlatlanderMale`. Additionally exposes:

- `getPeakAngle()` — returns the peak angle of the triangle.

The class is named `Isocele` in the current API. It represents an isosceles triangle, but the spelling is retained for API consistency with the project.

#### `Polygon`

Represents a male flatlander with multiple sides. Inherits from `FlatlanderMale`. Constructor accepts an additional `sides` parameter. Additionally exposes:

- `getNbSides()` — returns the number of sides.

### Comparison Operators

The following free comparison operators are available for `Flatlander` objects:

```cpp
bool operator<(const Flatlander &lhs, const Flatlander &rhs);
bool operator>(const Flatlander &lhs, const Flatlander &rhs);
bool operator<=(const Flatlander &lhs, const Flatlander &rhs);
bool operator>=(const Flatlander &lhs, const Flatlander &rhs);
bool operator==(const Flatlander &lhs, const Flatlander &rhs);
bool operator!=(const Flatlander &lhs, const Flatlander &rhs);
```

### Stream Output

`operator<<` is overloaded so a flatlander can be printed directly to any output stream:

```cpp
std::cout << segment << std::endl;
```

### `Family`

The `Family` class aggregates a family with parent-child relationships. It holds non-owning pointers to its members — the caller is responsible for lifetime management.

Key features:

- `newFamily(FlatlanderMale*, Segment*)` — static factory that validates the parents against the global registry.
- `adoptChild(Flatlander*)` — adds a child to the family.
- `newChild()` — creates a child from the family's parents; ownership transfers to the caller.
- `getChildCount()`, `getChildren()`, `findChild(const std::string&)` — query children.
- `getFather()`, `setFather()`, `getMother()`, `setMother()` — manage parents.
- `getName()`, `setName()` — family name.
- `present()` — prints the entire family.
- `findAll(Predicate*, int, int)` — searches children matching a predicate function.

The class maintains static registries (`registeredParents`, `registeredChildren`) to enforce Flatlandian society rules: no polygamy and no child with more than two parents. Copying and assignment are deleted.

### `RobotSurgeon`

`RobotSurgeon` has overloads for changing the colour of each shape type:

```cpp
flatland::RobotSurgeon surgeon{"Dr BES"};
surgeon.modifyColour(segment, flatland::Colour::BLUE);
```

It can also modify the peak angle of an `Isocele` and the number of sides of a `Polygon`:

```cpp
surgeon.modifyPeakAngle(triangle, 45);
surgeon.modifyNbSides(polygon, 6);
```

The surgeon is declared as a friend of the shape classes so these operations can update their private state.

### Predicates and `BindSecond`

The `Family::findAll()` method accepts a `Predicate` function pointer for filtering children. The `BindSecond` functor adapts a two-argument `CompIfPredicate` into a single-argument callable suitable for algorithms like `std::find_if()`:

```cpp
using Predicate = bool(const Flatlander &, int, int);
using CompIfPredicate = bool(const Flatlander*, int);
```

### `Colour`

`Colour` is a scoped enum with the following values:

```cpp
flatland::Colour::WHITE
flatland::Colour::BLUE
flatland::Colour::YELLOW
flatland::Colour::BLACK
flatland::Colour::RED
flatland::Colour::GREEN
flatland::Colour::PURPLE
```

Use `convertColourEnumToText()` when a readable colour name is needed:

```cpp
std::string text = flatland::convertColourEnumToText(
    flatland::Colour::GREEN);
```


### Custom Exceptions

The project defines `invalid_intarg`, a subclass of `std::invalid_argument`, for value validation errors:

```cpp
try {
    segment.setSize(200); // exceeds MAX_SIZE
} catch (const invalid_intarg& e) {
    std::cerr << e.what() << std::endl;
    int corrected = clamp(e.getInvalidValue(), e.getMax(), e.getMin());
}
```

It provides `getInvalidValue()`, `getMin()`, and `getMax()` so the caller can correct the value.

### Random Utilities

`util.hpp` provides helpers for generating random values:

```cpp
int value = flatland::randInt(1, 10);      // random int in [1, 10]
Colour c = flatland::randomColour();       // random colour enum value
```

These use a static `std::mt19937` generator seeded with `std::random_device`.

### Limits

The current utility constants define these intended limits:

| Property             | Minimum | Maximum |
| -------------------- | ------: | ------: |
| Shape size           |       5 |     100 |
| Isosceles peak angle |      20 |      60 |
| Polygon side count   |       4 |      20 |

Values are normalized by the project’s `clamp()` helper when changed through the relevant setter or surgeon operation.

## Design Notes

- Public declarations are kept in `include/` and implementations in `src/`.
- The `flatland` namespace prevents collisions with unrelated code.
- `Colour` is a scoped enum, so values must be written as `flatland::Colour::RED` rather than simply `RED`.
- Shape counts are maintained by the shape constructors and displayed by `printStatistics()`.
- The `Family` class uses static registries to enforce monogamy and unique parentage across all families.
- `invalid_intarg` exceptions are thrown when values exceed configured limits, allowing callers to clamp and retry.
- The current project is an educational example rather than a reusable geometry library. It does not currently provide unit tests, installation rules, package configuration, or a formal error-reporting API.

## Development Workflow

A typical edit-build-run cycle is:

```sh
cmake -S . -B build
cmake --build build
./build/flatland
```

To configure a fresh build directory:

```sh
rm -rf build
cmake -S . -B build
cmake --build build
```

Do not remove a build directory if it contains local work you need to preserve.

## Contributing

1. Create a branch for your change.
2. Keep public declarations in `include/` and implementations in `src/`.
3. Build the project with CMake.
4. Run the executable and verify the relevant output.
5. Keep generated build artifacts out of commits.
6. Open a pull request describing the change and its validation.

## License

No license has been added to this repository yet. Add a license before publishing or accepting contributions if you want to define how others may use, modify, and distribute the project.

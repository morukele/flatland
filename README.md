# Flatland

Flatland is a small C++23 geometry model inspired by Edwin A. Abbott's _Flatland_. It demonstrates basic object-oriented C++ concepts through geometric shapes, encapsulation, friend classes, namespaces, enum classes, validation helpers, and CMake builds.

## Features

- `Segment`, `Isocele`, and `Polygon` shape classes.
- Shape names, sizes, and colours with getters and setters.
- `RobotSurgeon`, a friend class that can modify protected shape properties.
- Size, angle, and polygon-side limits.
- Conversion of `Colour` enum values to readable text.
- Runtime statistics for the number of constructed shapes.
- A simple executable demonstration in `main.cpp`.

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
- `main.cpp`: demonstration program and sample usage.
- `include/flatlandien.hpp`: public shape and surgeon declarations.
- `include/util.hpp`: `Colour`, colour conversion, and clamping utilities.
- `src/flatlandien.cpp`: class method and statistics implementations.

Generated build files belong in `build/` and should not be committed to source control.

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

### Shape classes

Each shape currently provides:

- A constructor accepting a name, size, and colour.
- `getName()` and `setName()`.
- `getSize()` and `setSize()`.
- A colour getter.
- `present()` for console output.

`Isocele` additionally exposes `getPeakAngle()`, while `Polygon` exposes `getNbSides()`.

The class is named `Isocele` in the current API. It represents an isosceles triangle, but the spelling is retained for API consistency with the project.

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

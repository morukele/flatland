---
sessionId: session-261110-141731-jo05
---

# Requirements

### Overview
The `flatlandian` classes (`Flatlander`, `Segment`, `Isocele`, `Polygon`, `RobotSurgeon`, and `Family`) currently handle invalid parameters by silently clamping values to acceptable ranges using `clamp()`. The task is to replace this behavior with explicit `std::invalid_argument` exceptions so callers are notified when they pass invalid data.

### Scope
**In Scope:**
- All constructors and setter methods in `Flatlander`, `Segment`, `Isocele`, `Polygon`, `RobotSurgeon`, and `Family` that accept numeric parameters (size, angle, number of sides) and currently use `clamp()`.
- The `Flatlander::newFlatlander` factory method which also uses `clamp()`.

**Out of Scope:**
- Comparison operators (`operator<`, `>`, `<=`, `>=`, `==`, `!=`) — they already throw `std::invalid_argument` for incompatible types.
- Methods that already throw `std::logic_error` (e.g., `Family::newFamily`, `Family::newChild`) — these are business-rule violations, not parameter validation issues.
- The `clamp()` utility function itself — it will simply stop being called in the affected locations.

### Functional Requirements
- `Flatlander::setSize(int)` throws `std::invalid_argument` when size is outside `[MIN_SIZE, MAX_SIZE]`.
- `Flatlander` constructor throws when size is outside `[MIN_SIZE, MAX_SIZE]`.
- `Segment` constructor throws when size is outside `[MIN_SIZE, MAX_SIZE]`.
- `Isocele` constructor throws when size is outside `[MIN_SIZE, MAX_SIZE]`.
- `Polygon` constructor throws when size is outside `[MIN_SIZE, MAX_SIZE]` or sides is outside `[MIN_NB_SIZE, MAX_NB_SIZE]`.
- `RobotSurgeon::modifyPeakAngle(Isocele&, int)` throws when angle is outside `[MIN_ANGLE, MAX_ANGLE]`.
- `RobotSurgeon::modifyNbSides(Polygon&, int)` throws when sides is outside `[MIN_NB_SIZE, MAX_NB_SIZE]`.
- `Flatlander::newFlatlander` throws when computed child size is out of range (should not happen with valid parents, but defensive).


# Technical Design

### Current Implementation
In `src/flatlandien.cpp`, the following methods use `clamp()` from `util.hpp` to silently coerce out-of-range values:

| Method | Parameter | Current Behavior |
|---|---|---|
| `Flatlander::Flatlander(n, s, c)` | `s` (size) | `clamp(s, MAX_SIZE, MIN_SIZE)` |
| `Flatlander::setSize(s)` | `s` | `clamp(s, MAX_SIZE, MIN_SIZE)` |
| `Segment::Segment(n, s, c)` | `s` | `clamp(s, MAX_SIZE, MIN_SIZE)` |
| `Isocele::Isocele(n, s, c)` | `s` | `clamp(s, MAX_SIZE, MIN_SIZE)` |
| `Polygon::Polygon(n, s, c, sides)` | `s`, `sides` | `clamp(s, MAX_SIZE, MIN_SIZE)`, `clamp(sides, MAX_NB_SIZE, MIN_NB_SIZE)` |
| `RobotSurgeon::modifyPeakAngle(i, a)` | `a` | `clamp(a, MAX_ANGLE, MIN_ANGLE)` |
| `RobotSurgeon::modifyNbSides(p, n)` | `n` | `clamp(n, MAX_NB_SIZE, MIN_NB_SIZE)` |
| `Flatlander::newFlatlander(mother, father)` | computed `size` | `clamp(..., MAX_SIZE, MIN_SIZE)` |

The `<stdexcept>` header is already included, so `std::invalid_argument` is available.

### Key Decisions
- Use `std::invalid_argument` (not `std::out_of_range` or `std::logic_error`) for consistency with the existing comparison operators which already use this exception type.
- Validate in the constructor's initializer list where possible; for derived classes, validate before calling the base constructor.
- Include the parameter name and valid range in the exception message for debuggability.

### Proposed Changes
In `src/flatlandien.cpp`, replace each `clamp()` call with an explicit range check that throws `std::invalid_argument`:

1. **`Flatlander` constructor** — validate `s` before assignment in the member initializer.
2. **`Flatlander::setSize`** — throw if `s` is outside `[MIN_SIZE, MAX_SIZE]`.
3. **`Segment` constructor** — validate `s` before passing to base `Flatlander` constructor.
4. **`Isocele` constructor** — validate `s` before passing to base constructor.
5. **`Polygon` constructor** — validate both `s` and `sides` before passing to base constructor and initializing `nbSides`.
6. **`RobotSurgeon::modifyPeakAngle`** — throw if `a` is outside `[MIN_ANGLE, MAX_ANGLE]`.
7. **`RobotSurgeon::modifyNbSides`** — throw if `n` is outside `[MIN_NB_SIZE, MAX_NB_SIZE]`.
8. **`Flatlander::newFlatlander`** — validate computed child size; since parent sizes are guaranteed valid, the average should always be in range, but add a defensive check.

No changes to `include/flatlandien.hpp` are needed — no signatures change.


# Delivery Steps

### ✓ Step 1: Add validation to Flatlander, Segment, Isocele, and Polygon
Replace `clamp()` with `std::invalid_argument` in all shape constructors and `setSize()`.
- `Flatlander::Flatlander(n, s, c)`: throw if `s` outside `[MIN_SIZE, MAX_SIZE]`.
- `Flatlander::setSize(s)`: throw if `s` outside `[MIN_SIZE, MAX_SIZE]`.
- `Segment::Segment(n, s, c)`: throw if `s` outside `[MIN_SIZE, MAX_SIZE]` before calling base.
- `Isocele::Isocele(n, s, c)`: throw if `s` outside `[MIN_SIZE, MAX_SIZE]` before calling base.
- `Polygon::Polygon(n, s, c, sides)`: throw if `s` outside `[MIN_SIZE, MAX_SIZE]` or `sides` outside `[MIN_NB_SIZE, MAX_NB_SIZE]`.
- `Flatlander::newFlatlander`: add defensive check on computed child size.

### ✓ Step 2: Add validation to RobotSurgeon methods
Replace `clamp()` with `std::invalid_argument` in `RobotSurgeon` modification methods.
- `RobotSurgeon::modifyPeakAngle(i, a)`: throw if `a` outside `[MIN_ANGLE, MAX_ANGLE]`.
- `RobotSurgeon::modifyNbSides(p, n)`: throw if `n` outside `[MIN_NB_SIZE, MAX_NB_SIZE]`.
- Verify the project compiles and runs after all changes.

---

# New Requirements: Custom Exception and Correction in main()

### Overview
1. Create a custom exception class `flatland::invalid_intarg` that extends `std::invalid_argument` and provides access to the invalid value, minimum permitted value, and maximum permitted value.
2. Modify `main()` in `main.cpp` to catch `flatland::invalid_intarg` exceptions and correct the invalid values by clamping them to the permitted range, then retry the operation.

### Scope
**In Scope:**
- New `invalid_intarg` exception class in `include/util.hpp` (or a new header).
- Update all `std::invalid_argument` throws in `src/flatlandien.cpp` to use `flatland::invalid_intarg` instead.
- Modify `main()` to wrap test function calls in try-catch blocks that catch `flatland::invalid_intarg`, correct the value, and retry.

**Out of Scope:**
- `std::logic_error` exceptions (business rule violations) — these should not be caught and corrected.
- Comparison operator exceptions — these are type incompatibility errors, not value range errors.

### Functional Requirements
- `flatland::invalid_intarg` exception class:
  - Inherits from `std::invalid_argument`.
  - Stores: invalid value, min permitted, max permitted.
  - Provides getters: `getInvalidValue()`, `getMin()`, `getMax()`.
  - Overrides `what()` to provide a descriptive message.
- `main()` modification:
  - Wraps calls to test functions (Q1-Q13) in try-catch blocks.
  - Catches `flatland::invalid_intarg` exceptions.
  - Corrects the invalid value by clamping to [min, max].
  - Retries the operation with the corrected value.
  - Logs the correction to stderr for visibility.


# Technical Design: Custom Exception and Correction

### Current Implementation
- `src/flatlandien.cpp` throws `std::invalid_argument` with messages like "Flatlander size must be between 5 and 100".
- `main.cpp` currently calls Q13() directly without exception handling.
- The `clamp()` function in `util.hpp` can be used for correction in `main()`.

### Key Decisions
- Place `invalid_intarg` in `include/util.hpp` alongside other utilities, since it's a general-purpose exception for the flatland namespace.
- Inherit from `std::invalid_argument` to maintain compatibility with existing catch blocks.
- In `main()`, create a helper function `runWithCorrection()` that wraps test function calls and handles `invalid_intarg` exceptions.
- The correction strategy: when an `invalid_intarg` is caught, clamp the invalid value to [min, max] and retry the operation.

### Proposed Changes

#### 1. Add `invalid_intarg` exception class to `include/util.hpp`
```cpp
class invalid_intarg : public std::invalid_argument {
public:
    invalid_intarg(int invalid_value, int min_val, int max_val, const std::string& param_name);
    int getInvalidValue() const;
    int getMin() const;
    int getMax() const;
    const char* what() const noexcept override;
private:
    int invalid_value;
    int min_val;
    int max_val;
    std::string message;
};
```

#### 2. Update `src/flatlandien.cpp` to throw `flatland::invalid_intarg`
Replace all `std::invalid_argument` throws with `flatland::invalid_intarg`:
- `Flatlander` constructor and `setSize()`: throw `invalid_intarg(s, MIN_SIZE, MAX_SIZE, "size")`
- `Segment` constructor: throw `invalid_intarg(s, MIN_SIZE, MAX_SIZE, "size")`
- `Isocele` constructor: throw `invalid_intarg(s, MIN_SIZE, MAX_SIZE, "size")`
- `Polygon` constructor: throw `invalid_intarg(s, MIN_SIZE, MAX_SIZE, "size")` and `invalid_intarg(sides, MIN_NB_SIZE, MAX_NB_SIZE, "sides")`
- `RobotSurgeon::modifyPeakAngle()`: throw `invalid_intarg(a, MIN_ANGLE, MAX_ANGLE, "peakAngle")`
- `RobotSurgeon::modifyNbSides()`: throw `invalid_intarg(n, MIN_NB_SIZE, MAX_NB_SIZE, "nbSides")`
- `Flatlander::newFlatlander()`: throw `invalid_intarg(size, MIN_SIZE, MAX_SIZE, "childSize")`

#### 3. Modify `main()` in `main.cpp`
- Create a helper function that wraps test function execution with exception handling.
- Catch `flatland::invalid_intarg` exceptions.
- Correct the invalid value by clamping to [min, max].
- Retry the operation with the corrected value.
- Log corrections to stderr.


# Delivery Steps

### ✓ Step 1: Add validation to Flatlander, Segment, Isocele, and Polygon
Replace `clamp()` with `std::invalid_argument` in all shape constructors and `setSize()`.
- `Flatlander::Flatlander(n, s, c)`: throw if `s` outside `[MIN_SIZE, MAX_SIZE]`.
- `Flatlander::setSize(s)`: throw if `s` outside `[MIN_SIZE, MAX_SIZE]`.
- `Segment::Segment(n, s, c)`: throw if `s` outside `[MIN_SIZE, MAX_SIZE]` before calling base.
- `Isocele::Isocele(n, s, c)`: throw if `s` outside `[MIN_SIZE, MAX_SIZE]` before calling base.
- `Polygon::Polygon(n, s, c, sides)`: throw if `s` outside `[MIN_SIZE, MAX_SIZE]` or `sides` outside `[MIN_NB_SIZE, MAX_NB_SIZE]`.
- `Flatlander::newFlatlander`: add defensive check on computed child size.

### ✓ Step 2: Add validation to RobotSurgeon methods
Replace `clamp()` with `std::invalid_argument` in `RobotSurgeon` modification methods.
- `RobotSurgeon::modifyPeakAngle(i, a)`: throw if `a` outside `[MIN_ANGLE, MAX_ANGLE]`.
- `RobotSurgeon::modifyNbSides(p, n)`: throw if `n` outside `[MIN_NB_SIZE, MAX_NB_SIZE]`.
- Verify the project compiles and runs after all changes.

### ✓ Step 3: Create `flatland::invalid_intarg` exception class
Add the custom exception class to `include/util.hpp`.
- Define `invalid_intarg` class inheriting from `std::invalid_argument`.
- Store invalid value, min, and max permitted values.
- Implement constructor, getters, and `what()` override.
- Update `src/flatlandien.cpp` to throw `flatland::invalid_intarg` instead of `std::invalid_argument`.

### ✓ Step 4: Modify `main()` to catch and correct invalid arguments
Wrap test function calls in exception handling that corrects invalid values.
- Create a helper function in `main.cpp` that catches `flatland::invalid_intarg`.
- When caught, clamp the invalid value to [min, max] and log the correction.
- Retry the operation with the corrected value.
- Update `main()` to use the helper function for test execution.
- Verify the project compiles and runs with corrected values.
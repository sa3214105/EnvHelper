# EnvHelper

A C++20 header-only library for type-safe environment variable access, with or without default values. Supports int, float, double, long, long long, long double, char, and std::string_view.

> **Note:** This library requires a C++20-compliant compiler and standard library.

## Features
- Compile-time environment variable name via `FixedString`
- Type-safe conversion with exception handling
- Optional default value fallback
- Doxygen documentation
- MIT License

## Usage

### 1. With Default Value
```cpp
#include "library.h"
constexpr FixedString<5> envName("PATH");
constexpr DefaultValueType<int> defaultVal(42);
int value = EnvHelperWithDefault<envName, defaultVal>::get();
```
If the environment variable `PATH` is not set or conversion fails, `value` will be `42`.

### 2. Without Default Value
```cpp
#include "library.h"
constexpr FixedString<5> envName("PATH");
int value = EnvHelper<envName, int>::get();
```
If the environment variable `PATH` is not set or conversion fails, an exception will be thrown.

### 3. String Example
```cpp
#include "library.h"
constexpr FixedString<5> envName("HOME");
constexpr DefaultValueType<char, 6> defaultStr("/tmp/");
auto home = EnvHelperWithDefault<envName, defaultStr>::get(); // returns std::string_view
```

## Supported Types
- int, long, long long
- float, double, long double
- char
- std::string_view (for char arrays)

## License

This project is licensed under the [MIT License](./LICENSE).

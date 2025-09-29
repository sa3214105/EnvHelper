# EnvHelper

A C++20 header-only library for type-safe environment variable access, with or without default values. Supports int, float, double, long, long long, long double, char, and std::string_view.

> **Note:** This library requires a C++20-compliant compiler and standard library.

## Features
- Doxygen documentation
- MIT License

## Usage

### 1. With Default Value
```cpp
#include "EnvHelper.hpp"
auto url = EnvHelperWithDefault<"DATABASE_URL", "localhost">::get();
auto port = EnvHelperWithDefault<"PORT", 42>::get();
```
If the environment variable `PATH` is not set or conversion fails, `value` will be `42`.

### 2. Without Default Value
```cpp
#include "EnvHelper.hpp"
auto url = EnvHelper<"DATABASE_URL", "localhost">::get();
auto port = EnvHelper<"PORT", 42>::get();
```
If the environment variable `PATH` is not set or conversion fails, an exception will be thrown.

### 3. Using with Type Alias (Recommended: config.h)
For better maintainability, it is recommended to put all your environment variable type aliases in `config.h`:

```cpp
// config.h
#include "EnvHelper.hpp"
using DBUrl = EnvHelperWithDefault<"DATABASE_URL", "/tmp/db.sqlite">;
using Port = EnvHelperWithDefault<"PORT", 8080>;
```

Then, in other files, simply include `config.h` and use the aliases:

```cpp
#include "config.h"
auto url = DBUrl::get();
auto port = Port::get();
```
This approach keeps your code cleaner and makes it easier to manage environment variable access across your project.

### 4. Disabling Messages
To disable all automatic output messages, set the third template parameter to `false`:

```cpp
#include "EnvHelper.hpp"
auto url = EnvHelperWithDefault<"DATABASE_URL", "localhost", false>::get();
auto port = EnvHelper<"PORT", 42, false>::get();
```
No messages will be printed to stdout or stderr when environment variables are accessed or converted.

## Supported Types
- int, long, long long
- float, double, long double
- char
- std::string_view (for char arrays)

## License

This project is licensed under the [MIT License](./LICENSE).

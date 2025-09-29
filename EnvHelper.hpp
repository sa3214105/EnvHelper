// MIT License © 2025 sa3214105
// See LICENSE file in the project root for full license text.
/**
 * @file library.h
 * @brief Environment variable helper classes with and without default value support.
 *
 * Provides FixedString, DefaultValueType, EnvHelperWithDefault, and EnvHelper for type-safe environment variable access.
 */
#pragma once
#include <iostream>
#include <string_view>
#include <stdexcept>

/**
 * @brief Stores a default value for environment variable access.
 * @tparam T Value type (char, floating point, or integral)
 * @tparam N Size for char arrays (default 0)
 */
template<typename T, size_t N = 0>
    requires std::same_as<char,T> || std::is_floating_point_v<T> || std::is_integral_v<T>
struct DefaultValueType {
    using SaveType = std::conditional_t<std::is_same_v<char, T> && N != 0, char [N], T>;
    using ExportType = std::conditional_t<std::is_same_v<char, T> && N != 0, std::string_view, T>;
    SaveType value{};

    /**
     * @brief Construct from a value.
     * @param val The default value.
     */
    constexpr DefaultValueType(T val) { value = val; }

    /**
     * @brief Construct from a string literal.
     * @param str The default string.
     */
    constexpr DefaultValueType(const T (&str)[N]) {
        for (size_t i = 0; i < N; ++i)
            value[i] = str[i];
    }

    /**
     * @brief Get the stored value in export format.
     * @return ExportType value.
     */
    constexpr ExportType get() const {
        if constexpr (std::is_same_v<char, T> && N != 0) {
            return {value, N - 1};
        } else {
            return value;
        }
    }
};

/**
 * @brief Compile-time fixed string for template parameters.
 * @tparam N String size (including null terminator)
 */
template<size_t N = 0>
struct FixedString {
    char value[N];

    /**
     * @brief Construct from a string literal.
     * @param str The string literal.
     */
    constexpr FixedString(const char (&str)[N]) {
        for (size_t i = 0; i < N; ++i)
            value[i] = str[i];
    }

    /**
     * @brief Convert to std::string_view.
     * @return String view of the value (excluding null terminator).
     */
    constexpr operator std::string_view() const {
        return {value, N - 1};
    }
};

/**
 * @brief Environment variable helper with default value fallback.
 * @tparam EnvName FixedString environment variable name
 * @tparam DefaultValue DefaultValueType default value
 * @tparam EnableValueMessage Print value messages if true
 */
template<FixedString EnvName, DefaultValueType DefaultValue, bool EnableValueMessage = true>
class EnvHelperWithDefault {
public:
    /**
     * @brief Get the environment variable value, or default if not set or conversion fails.
     * @return Value of type DefaultValue::ExportType
     */
    static auto &get() {
        static auto value = []() {
            const char *env = std::getenv(EnvName.value);
            if (env == nullptr) {
                if constexpr (EnableValueMessage) {
                    std::cout << EnvName.value << " is not set, using default: " << DefaultValue << "\n";
                }
                return DefaultValue.get();
            }
            if constexpr (EnableValueMessage) {
                std::cout << EnvName.value << " is set to: " << env << "\n";
            }
            using exportType = decltype(DefaultValue)::ExportType;
            try {
                if constexpr (std::is_same_v<exportType, int>) {
                    return std::stoi(env);
                } else if constexpr (std::is_same_v<exportType, long>) {
                    return std::stol(env);
                } else if constexpr (std::is_same_v<exportType, long long>) {
                    return std::stoll(env);
                } else if constexpr (std::is_same_v<exportType, float>) {
                    return std::stof(env);
                } else if constexpr (std::is_same_v<exportType, double>) {
                    return std::stod(env);
                } else if constexpr (std::is_same_v<exportType, long double>) {
                    return std::stold(env);
                } else if constexpr (std::is_same_v<exportType, char>) {
                    return env[0];
                } else {
                    return std::string_view(env);
                }
            } catch (const std::exception& e) {
                std::cerr << "[EnvHelper] " << EnvName.value << " conversion failed: (env='" << env << "'), using default value: " << DefaultValue.get() << std::endl;
                std::cerr << e.what() << std::endl;
                return DefaultValue.get();
            }
        }();
        return value;
    }
};

/**
 * @brief Environment variable helper without default value. Throws if not set or conversion fails.
 * @tparam EnvName FixedString environment variable name
 * @tparam Type Target type
 * @tparam EnableValueMessage Print value messages if true
 */
template<FixedString EnvName, typename Type, bool EnableValueMessage = true>
class EnvHelper {
public:
    /**
     * @brief Get the environment variable value. Throws if not set or conversion fails.
     * @return Value of type Type
     * @throws std::runtime_error if not set or conversion fails
     */
    static Type get() {
        const char *env = std::getenv(EnvName.value);
        if (env == nullptr) {
            std::cerr << EnvName.value << " is not set and no default value is provided.\n";
            throw std::runtime_error(std::string(EnvName.value) + " is not set and no default value is provided.");
        }
        if constexpr (EnableValueMessage) {
            std::cout << EnvName.value << " is set to: " << env << "\n";
        }
        try {
            if constexpr (std::is_same_v<Type, int>) {
                return std::stoi(env);
            } else if constexpr (std::is_same_v<Type, long>) {
                return std::stol(env);
            } else if constexpr (std::is_same_v<Type, long long>) {
                return std::stoll(env);
            } else if constexpr (std::is_same_v<Type, float>) {
                return std::stof(env);
            } else if constexpr (std::is_same_v<Type, double>) {
                return std::stod(env);
            } else if constexpr (std::is_same_v<Type, long double>) {
                return std::stold(env);
            } else if constexpr (std::is_same_v<Type, char>) {
                return env[0];
            } else if constexpr (std::is_same_v<Type, std::string_view>) {
                return std::string_view(env);
            } else {
                static_assert(sizeof(Type) == 0, "Unsupported type for EnvHelperNoDefault");
            }
        } catch (const std::exception& e) {
            std::cerr << "[EnvHelperNoDefault] " << EnvName.value << " conversion failed: (env='" << env << "')" << std::endl;
            std::cerr << e.what() << std::endl;
            throw;
        }
    }
};

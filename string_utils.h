/*
 * string_utils.h
 *
 *  Created on: 27 lut 2020
 *      Author: emvi
 */

#ifndef STRING_UTILS_H_
#define STRING_UTILS_H_

#include <string>

constexpr uint32_t val_32_const = 0x811c9dc5;
constexpr uint32_t prime_32_const = 0x1000193;
constexpr uint64_t val_64_const = 0xcbf29ce484222325;
constexpr uint64_t prime_64_const = 0x100000001b3;

constexpr uint32_t fnv1a(const char *const str, const uint32_t value = val_32_const) noexcept {
    return (str[0] == '\0')
        ? value
        : fnv1a(&str[1], (value ^ uint32_t(str[0])) * prime_32_const);
}

constexpr uint32_t operator"" _h(const char *str, size_t) { return fnv1a(str); }

#endif /* STRING_UTILS_H_ */

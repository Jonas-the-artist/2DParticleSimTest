//
//  defines.h
//  HelloOpenGL
//
//  Created by Jonas Büchner on 18.04.23.
//

#pragma once
#include <cstdint>
#include <vector>
#include "glm/vec2.hpp"

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef float float32;
typedef double float64;

struct Line {
    glm::vec2 start;
    glm::vec2 end;

    Line(const glm::vec2& start, const glm::vec2& end)
        : start(start), end(end) {}
};

#pragma once

#include <cstdint>

// specifying uint8_t for less memory usage 
// limit of 256 block types

enum BlockID : uint8_t {
    AIR = 0,
    STONE,
    DIRT,
    GRASS
};

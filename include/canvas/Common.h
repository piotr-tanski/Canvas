#pragma once

#include <cstdint>

namespace canvas {

    using Color = std::uint8_t;
    namespace colors {
        static constexpr inline Color White = 0;
        static constexpr inline Color Black = 1;
    } // namespace colors

    using Length = unsigned int;

    struct Point {
        unsigned int x;
        unsigned int y;
    };

    struct CanvasResolution {
        std::uint16_t height;
        std::uint16_t width;
    };

} // namespace canvas

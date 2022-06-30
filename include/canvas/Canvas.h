#pragma once

#include "Common.h"

#include <vector>

namespace canvas {

namespace shapes {
    class Shape; // forward declaration
} // namespace shapes

class Canvas {
public:
    using DrawingArea = std::vector<Color>;

    explicit Canvas(CanvasResolution resolution);
    virtual ~Canvas() noexcept = default;

    virtual void draw(shapes::Shape *shape) = 0;
    virtual void erase(shapes::Shape *shape) = 0;

    [[nodiscard]] const DrawingArea& getData() const noexcept { return area; }

protected:
    DrawingArea area;
};

namespace painting {
    class Tool {
    public:
        virtual ~Tool() noexcept = default;

        virtual void apply(Point point, Color color) = 0;
    };
} // namespace painting

} // namespace canvas
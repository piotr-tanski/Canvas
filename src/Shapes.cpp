#include <canvas/Shapes.h>

#include <cmath>

namespace canvas::shapes {

namespace {
    using Radians = double;
    using Degrees = unsigned int;
    constexpr Radians toRadians(Degrees degrees) { return degrees * M_PI / 180; }
} // namespace

Rectangle::Rectangle(Rectangle::Params params) : attributes{params} {}

void Rectangle::setAttributes(Params params) noexcept {
    changeAttributes<Rectangle>(attributes, params, onChange, this);
}

void Rectangle::changeImpl()  {
    attributes.change();
}

void Rectangle::draw(painting::Tool *tool) {
    const auto startX = attributes.get().start.x;
    const auto startY = attributes.get().start.y;
    for (auto r = 0U; r < attributes.get().height; ++r) {
        for (auto c = 0U; c < attributes.get().width; ++c) {
            tool->apply({startX + c, startY + r}, attributes.get().color);
        }
    }
}

Circle::Circle(Params params) : attributes{params} {}

void Circle::setAttributes(Params params) noexcept {
    changeAttributes<Circle>(attributes, params, onChange, this);
}

void Circle::changeImpl()  {
    attributes.change();
}

void Circle::draw(painting::Tool *tool) {
    const auto start = toRadians(0);
    const auto end   = toRadians(360);
    const auto radius = attributes.get().radius;
    double step = 0.001;

    unsigned int x, y;
    for (double radians = start; radians <= end; radians += step) {
        x = std::lround(std::cos(radians) * radius); // adjacent side of triangle
        y = std::lround(std::sin(radians) * radius); // opposite side of triangle
        tool->apply(Point{attributes.get().center.x + x, attributes.get().center.y + y}, attributes.get().color);
    }
}

Triangle::Triangle(Params params) : attributes{params} {}

void Triangle::setAttributes(Params params) noexcept {
    changeAttributes<Triangle>(attributes, params, onChange, this);
}

void Triangle::changeImpl()  {
    attributes.change();
}


void Triangle::draw(painting::Tool *tool) {
    drawLine(attributes.get().v1, attributes.get().v2, tool);
    drawLine(attributes.get().v1, attributes.get().v3, tool);
    drawLine(attributes.get().v2, attributes.get().v3, tool);
}

void Triangle::drawLine(Point a, Point b, painting::Tool *tool) {
    const auto distanceX = b.x > a.x ? b.x - a.x : a.x - b.x;
    const auto distanceY =  b.y > a.y ? b.y - a.y : a.y - b.y;
    const auto step     = distanceX >= distanceY ? distanceX : distanceY;

    auto dx = static_cast<float>(distanceX) / step;
    dx      = b.x < a.x ? -dx : dx;
    auto dy = static_cast<float>(distanceY) / step;
    dy      = b.y < a.y ? -dy : dy;

    auto x = static_cast<float>(a.x);
    auto y = static_cast<float>(a.y);
    for (unsigned int i = 0; i < step; ++i) {
        tool->apply(Point{static_cast<unsigned int>(x), static_cast<unsigned int>(y)}, attributes.get().color);
        x += dx;
        y += dy;
    }
}

} // namespace canvas::shapes
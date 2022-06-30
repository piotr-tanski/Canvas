#include <canvas/Shapes.h>

namespace canvas::shapes {

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

}

Triangle::Triangle(Params params) : attributes{params} {}

void Triangle::setAttributes(Params params) noexcept {
    changeAttributes<Triangle>(attributes, params, onChange, this);
}

void Triangle::changeImpl()  {
    attributes.change();
}


void Triangle::draw(painting::Tool *tool) {

}

} // namespace canvas::shapes
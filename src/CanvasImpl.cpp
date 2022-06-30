#include "CanvasImpl.h"

#include <memory>

namespace canvas {

namespace {
constexpr Color DefaultColor = colors::White;
} // namespace

Canvas::Canvas(CanvasResolution resolution) : area(resolution.height * resolution.width, DefaultColor) {}

CanvasImpl::CanvasImpl(CanvasResolution resolution) : Canvas(resolution), resolution(resolution) {}

CanvasImpl::~CanvasImpl() noexcept {
    for (auto obj : objects) {
        delete obj;
    }
}

void CanvasImpl::draw(shapes::Shape *shape) {
    drawImpl(shape);
    objects.push_back(shape);
}

void CanvasImpl::drawImpl(shapes::Shape *shape) {
    checkIfAreaIsEmpty(shape);

    auto brush = std::make_unique<Brush>(*this);
    shape->draw(brush.get());
}

void CanvasImpl::erase(shapes::Shape *shape) {
    eraseImpl(shape);
    objects.remove(shape);
}

void CanvasImpl::eraseImpl(shapes::Shape *shape) {
    auto rubber = std::make_unique<Eraser>(*this);
    shape->draw(rubber.get());
}

void CanvasImpl::checkIfAreaIsEmpty(shapes::Shape *shape) {
    auto sketcher = std::make_unique<Sketcher>(*this);
    shape->draw(sketcher.get());
}

unsigned int CanvasImpl::pointToPosition(Point point) const noexcept { return point.x + resolution.width * point.y; }

void CanvasImpl::checkIfInBounds(unsigned int position) const {
    if (position >= area.size()) {
        throw OutOfCanvasError{"Drawing outside of canvas"};
    }
}

Brush::Brush(CanvasImpl &canvas) : canvas{canvas} {}

void Brush::apply(Point point, Color color) {
    const auto pos = canvas.pointToPosition(point);
    canvas.checkIfInBounds(pos);
    canvas.area[pos] = color;
}

Eraser::Eraser(CanvasImpl &canvas) : canvas{canvas} {}

void Eraser::apply(Point point, [[maybe_unused]] Color color) {
    const auto pos = canvas.pointToPosition(point);
    canvas.checkIfInBounds(pos);
    canvas.area[pos] = DefaultColor;
}

Sketcher::Sketcher(CanvasImpl &canvas) : canvas{canvas} {}

void Sketcher::apply(Point point, [[maybe_unused]] Color color) {
    const auto pos = canvas.pointToPosition(point);
    canvas.checkIfInBounds(pos);

    const auto content = canvas.area[pos];
    if (content != DefaultColor) {
        throw ObjectsOverlappingError{"Unable to draw object: objects are overlapping"};
    }
}

} // namespace canvas
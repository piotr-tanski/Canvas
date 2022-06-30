#include "CanvasImpl.h"

#include <stdexcept>

namespace canvas {

namespace {
    constexpr Color DefaultColor = colors::White;

    class OutOfCanvasError : public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };

    class ObjectsOverlappingError : public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };
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
    if (isAreaPreparedFor(shape)) {
        auto brush = std::make_unique<Brush>(*this);
        shape->draw(brush.get());
    }
}

void CanvasImpl::erase(shapes::Shape *shape) {
    eraseImpl(shape);
    objects.remove(shape);
}

void CanvasImpl::eraseImpl(shapes::Shape *shape) {
    auto rubber = std::make_unique<Eraser>(*this);
    shape->draw(rubber.get());
}

bool CanvasImpl::isAreaPreparedFor(shapes::Shape *shape) {
    auto sketcher = std::make_unique<Sketcher>(*this);
    try {
        shape->draw(sketcher.get());
    }
    catch (const ObjectsOverlappingError &) {
        return false;
    }
    return true;
}

unsigned int CanvasImpl::pointToPosition(Point point) const noexcept {
    return point.x + resolution.width * point.y;
}

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
        throw ObjectsOverlappingError{"Objects are overlapping"};
    }
}

} // namespace canvas
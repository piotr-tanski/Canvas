#pragma once

#include <canvas/Canvas.h>
#include <canvas/Shapes.h>

#include <list>

namespace canvas {

class CanvasImpl : public Canvas {
    using Objects = std::list<shapes::Shape*>;
public:
    explicit CanvasImpl(CanvasResolution resolution);
    ~CanvasImpl() noexcept override;

    void draw(shapes::Shape *shape) override;
    void erase(shapes::Shape *shape) override;

private:
    void drawImpl(shapes::Shape *shape);
    void eraseImpl(shapes::Shape *shape);
    bool isAreaPreparedFor(shapes::Shape *shape);

    CanvasResolution resolution;
    Objects objects;

    friend class Brush;
    friend class Eraser;
    friend class Sketcher;
};

class Brush : public painting::Tool {
public:
    explicit Brush(CanvasImpl& canvas);

    void apply(Point point, Color color) override;

private:
    CanvasImpl& canvas;
};

class Eraser : public painting::Tool {
public:
    explicit Eraser(CanvasImpl& canvas);

    void apply(Point point, Color color) override;

private:
    CanvasImpl& canvas;
};

class Sketcher : public painting::Tool {
public:
    explicit Sketcher(CanvasImpl& canvas);

    void apply(Point point, Color color) override;

private:
    CanvasImpl& canvas;
};

} // namespace canvas

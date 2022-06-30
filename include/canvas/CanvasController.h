#pragma once

#include "Canvas.h"
#include "Shapes.h"

#include <list>
#include <memory>

namespace canvas {

struct DrawCommand {
    enum class Type { None, Add, Remove, Change, Select, Unselect };

    Type type = Type::None;
    shapes::Shape *target = nullptr;
};

class Batch {
  public:
    using CommandList = std::list<DrawCommand>;

    Batch &add(shapes::Shape *shape);
    Batch &remove(shapes::Shape *shape);
    Batch &change(shapes::MutableShape *shape);
    Batch &select(shapes::MutableShape *shape);
    Batch &unselect(shapes::MutableShape *shape);

    [[nodiscard]] const CommandList &getCommands() const noexcept;

  private:
    CommandList commands;
};

class CanvasController {
  public:
    explicit CanvasController(std::unique_ptr<Canvas> &&canvas);

    void execute(Batch &&batch);

    [[nodiscard]] Canvas::DrawingArea getCanvasData() const noexcept;

  private:
    void add(shapes::Shape *shape);
    void remove(shapes::Shape *shape);
    void change(shapes::MutableShape *shape);

    std::unique_ptr<Canvas> canvas;
};

class CanvasControllerFactory {
  public:
    static std::unique_ptr<CanvasController> create(CanvasResolution resolution);

  private:
    static std::unique_ptr<Canvas> createDefaultCanvas(CanvasResolution resolution);
};

} // namespace canvas
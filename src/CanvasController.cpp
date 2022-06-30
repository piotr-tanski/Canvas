#include <canvas/CanvasController.h>
#include "CanvasImpl.h"

namespace canvas {

Batch& Batch::add(shapes::Shape *shape) {
    commands.push_back(DrawCommand{DrawCommand::Type::Add, shape});
    return *this;
}

Batch& Batch::remove(shapes::Shape *shape) {
    commands.push_back(DrawCommand{DrawCommand::Type::Remove, shape});
    return *this;
}

Batch &Batch::change(shapes::MutableShape *shape) {
    commands.push_back(DrawCommand{DrawCommand::Type::Change, shape});
    return *this;
}

Batch &Batch::select(shapes::MutableShape *shape) {
    commands.push_back(DrawCommand{DrawCommand::Type::Select, shape});
    return *this;
}

Batch &Batch::unselect(shapes::MutableShape *shape) {
    commands.push_back(DrawCommand{DrawCommand::Type::Unselect, shape});
    return *this;
}

const Batch::CommandList &Batch::getCommands() const noexcept {
    return commands;
}

std::unique_ptr<CanvasController> CanvasControllerFactory::create(CanvasResolution resolution) {
    return std::make_unique<CanvasController>(createDefaultCanvas(resolution));
}

std::unique_ptr<Canvas> CanvasControllerFactory::createDefaultCanvas(CanvasResolution resolution) {
    return std::make_unique<CanvasImpl>(resolution);
}

CanvasController::CanvasController(std::unique_ptr<Canvas> &&canvas) : canvas{std::move(canvas)} {}

void CanvasController::execute(Batch &&batch) {
    for (const auto &cmd : batch.getCommands()) {
        switch (cmd.type) {
            case DrawCommand::Type::Add:
                add(cmd.target);
                break;
            case DrawCommand::Type::Remove:
                remove(cmd.target);
                break;
            case DrawCommand::Type::Change: {
                auto target = dynamic_cast<shapes::MutableShape *>(cmd.target);
                change(target);
                break;
            }
            case DrawCommand::Type::Select: {
                auto target = dynamic_cast<shapes::MutableShape *>(cmd.target);
                target->bindOnChange([this](shapes::MutableShape *shape) {
                    change(shape);
                });
                break;
            }
            case DrawCommand::Type::Unselect: {
                auto target = dynamic_cast<shapes::MutableShape *>(cmd.target);
                target->bindOnChange(nullptr);
                break;
            }
            default:
                break;
        }
    }
}

void CanvasController::add(shapes::Shape *shape) {
    if (shape != nullptr) {
        canvas->draw(shape);
    }
}

void CanvasController::remove(shapes::Shape *shape) {
    if (shape != nullptr) {
        canvas->erase(shape);
    }
}

void CanvasController::change(shapes::MutableShape *shape) {
    if (shape == nullptr) {
        return;
    }

    remove(shape);
    shape->change();
    add(shape);
}

Canvas::DrawingArea CanvasController::getCanvasData() const noexcept {
    return canvas->getData();
}

} // namespace canvas
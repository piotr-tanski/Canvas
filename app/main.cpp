#include <iostream>

#include <canvas/Canvas.h>
#include <canvas/CanvasController.h>
#include <canvas/Shapes.h>

constexpr auto resolution = canvas::CanvasResolution{20, 20};

void displayCanvas(const canvas::Canvas::DrawingArea& canvas, std::string &&title) {
    std::cout << '\n' << title << '\n';
    for (auto r = 0; r < resolution.height; ++r) {
        for (auto c = 0; c < resolution.width; ++c) {
            std::cout << static_cast<int>(canvas[c + resolution.width * r]);
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

int main(int, char**) {
    auto controller = canvas::CanvasControllerFactory::create(resolution);

    using canvas::shapes::Rectangle;
    Rectangle::Params params{{2, 3}, 6, 5, canvas::colors::Black};
    auto obj = new Rectangle{params};

    // Display rectangle
    canvas::Batch displayCmd;
    displayCmd.add(obj);
    controller->execute(std::move(displayCmd));
    displayCanvas(controller->getCanvasData(), "Display 6x5 rectangle");

    // Add overlapping rectangle
    auto overlappingParams = params;
    overlappingParams.start = canvas::Point{0, 0};
    auto overlapping = new Rectangle{overlappingParams};

    canvas::Batch overlappingObjCmd;
    overlappingObjCmd.add(overlapping);
    controller->execute(std::move(overlappingObjCmd));
    displayCanvas(controller->getCanvasData(), "Display canvas (overlapping object not added)");

    // Modify selected rectangle
    canvas::Batch selectCmd;
    selectCmd.select(obj);
    controller->execute(std::move(selectCmd));

    params.width = 4;
    params.height = 8;
    obj->setAttributes(params);
    displayCanvas(controller->getCanvasData(), "Changed selected rectangle to 4x8");

    // Unselect
    canvas::Batch unselectCmd;
    unselectCmd.unselect(obj);
    controller->execute(std::move(unselectCmd));

    // Modify rectangle
    params.width = 1;
    params.height = 2;
    obj->setAttributes(params);
    displayCanvas(controller->getCanvasData(), "Changed unselected rectangle to 1x2");

    canvas::Batch updateCmd;
    updateCmd.change(obj);
    controller->execute(std::move(updateCmd));
    displayCanvas(controller->getCanvasData(), "Explicitly changed unselected rectangle to 1x2");

    // Modify rectangle
    params.width = 0;
    params.height = 0;
    obj->setAttributes(params);

    canvas::Batch updateCmd0x0;
    updateCmd0x0.change(obj);
    controller->execute(std::move(updateCmd0x0));
    displayCanvas(controller->getCanvasData(), "Explicitly changed unselected rectangle to 0x0 (not visible)");

    // Remove rectangle
    canvas::Batch removeCmd;
    removeCmd.remove(obj);
    controller->execute(std::move(removeCmd));
    displayCanvas(controller->getCanvasData(), "Removed rectangle");

    return 0;
}
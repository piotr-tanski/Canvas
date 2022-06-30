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

void testRectangles() {
    auto controller = canvas::CanvasControllerFactory::create(resolution);

    using canvas::shapes::Rectangle;
    Rectangle::Params params{{1, 1}, 6, 8, canvas::colors::Black};
    auto obj = new Rectangle(params);

    canvas::Batch displayCmd;
    displayCmd.add(obj);
    controller->execute(std::move(displayCmd));
    displayCanvas(controller->getCanvasData(), "Display a rectangle");

    auto overlappingParams = params;
    overlappingParams.start = canvas::Point{0, 0};
    auto overlapping = new Rectangle{overlappingParams};

    canvas::Batch overlappingObjCmd;
    overlappingObjCmd.add(overlapping);
    try {
        controller->execute(std::move(overlappingObjCmd));
    }
    catch (const canvas::ObjectsOverlappingError &error) {
        std::cout << "Error occurred: " << error.what() << '\n';
        delete overlapping;
    }

    // Modify selected rectangle
    canvas::Batch selectCmd;
    selectCmd.select(obj);
    controller->execute(std::move(selectCmd));

    params.width = 4;
    params.height = 2;
    obj->setAttributes(params);
    displayCanvas(controller->getCanvasData(), "Changed selected rectangle to 4x2");

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
}

void testTriangles() {
    auto controller = canvas::CanvasControllerFactory::create(resolution);

    using canvas::shapes::Triangle;
    Triangle::Params params{{1, 1}, {5, 1}, {1, 5}, canvas::colors::Black};
    auto obj = new Triangle(params);

    canvas::Batch displayCmd;
    displayCmd
            .add(obj)
            .select(obj);
    controller->execute(std::move(displayCmd));
    displayCanvas(controller->getCanvasData(), "Display a triangle");

    params.v2 = canvas::Point{8, 1};
    obj->setAttributes(params);
    displayCanvas(controller->getCanvasData(), "Display bigger triangle");
}

void testCircles() {
    auto controller = canvas::CanvasControllerFactory::create(resolution);

    using canvas::shapes::Circle;
    Circle::Params params{{7, 7}, 6, canvas::colors::Black};
    auto obj = new Circle(params);

    canvas::Batch displayCmd;
    displayCmd
            .add(obj)
            .select(obj);
    controller->execute(std::move(displayCmd));
    displayCanvas(controller->getCanvasData(), "Display a circle");

    params.radius = 3;
    obj->setAttributes(params);
    displayCanvas(controller->getCanvasData(), "Display smaller circle");
}

int main(int, char**) {
    testRectangles();
    testTriangles();
    testCircles();
    return 0;
}
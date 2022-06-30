set(HEADERS_LIST
        "${Canvas_SOURCE_DIR}/include/canvas/Canvas.h"
        "${Canvas_SOURCE_DIR}/include/canvas/CanvasController.h"
        "${Canvas_SOURCE_DIR}/include/canvas/Shapes.h")

add_library(
    canvas
    CanvasController.cpp
    CanvasImpl.cpp
    CanvasImpl.h
    Shapes.cpp
    ${HEADERS_LIST})

target_include_directories(canvas PUBLIC ../include)
target_compile_features(canvas PUBLIC cxx_std_20)

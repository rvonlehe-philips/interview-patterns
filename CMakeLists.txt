cmake_minimum_required(VERSION 3.16)

project(pattern-examples LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(visitor
    #[[main.cpp
    Point.h Circle.h
    Square.h
    DrawAllShapes.cpp
    DrawAllShapes.h
    Shapes.h
    Shape.h
    Draw.h]]
    visitor.cpp
)

target_include_directories(visitor PUBLIC .)

include(GNUInstallDirs)
install(TARGETS visitor
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

add_executable(observer
    observer.cpp
)

add_executable(strategy
    strategy.cpp
)

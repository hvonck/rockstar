CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

# Create the project.
PROJECT(rockstar)

SET(Sources "src/generic.cc" "src/generic.h" "src/instruction.h" "src/interpreter.cc" "src/interpreter.h" "src/scope.cc" "src/scope.h" "src/stack.cc" "src/stack.h" "src/variable.cc" "src/variable.h")
ADD_LIBRARY(rockstar ${Sources})

ADD_EXECUTABLE(rockstar-test "src/main.cc")
TARGET_LINK_LIBRARIES(rockstar-test rockstar)

cmake_minimum_required(VERSION 3.16...3.21)
project(kerma)

find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
file(GLOB MACRO_FILES
    "*.mac"
)

file(COPY ${MACRO_FILES} DESTINATION ${PROJECT_BINARY_DIR})

add_executable(sim sim.cc ${sources} ${headers})
target_link_libraries(sim ${Geant4_LIBRARIES})

add_custom_target(kerma DEPENDS sim)

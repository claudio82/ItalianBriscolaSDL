cmake_minimum_required(VERSION 3.10)
project(ItalianBriscolaSDL)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_ttf REQUIRED)

include_directories(
    ${PROJECT_SOURCE_DIR}
    INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
              $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/${PROJECT_NAME}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

add_executable(${PROJECT_NAME} src/AssetManager.cpp src/ErrorMessage.cpp src/TextRenderer.cpp src/TextureManager.cpp src/Timer.cpp src/TextRendererTimer.cpp src/Briscola.cpp src/Game.cpp src/Source.cpp)

target_link_libraries(${PROJECT_NAME} SDL3::SDL3)
target_link_libraries(${PROJECT_NAME} SDL3_image::SDL3_image)
target_link_libraries(${PROJECT_NAME} SDL3_ttf::SDL3_ttf)
